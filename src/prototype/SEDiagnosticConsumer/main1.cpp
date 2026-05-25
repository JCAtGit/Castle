#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTStructuralEquivalence.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Serialization/PCHContainerOperations.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace clang;

// Helper function to extract the first target structure definition found in a parsed file
RecordDecl *getFirstRecordDecl(ASTContext &Context) {
  TranslationUnitDecl *TUD = Context.getTranslationUnitDecl();
  for (Decl *D : TUD->decls()) {
    if (auto *RD = dyn_cast<RecordDecl>(D)) {
      // Return only actual struct definitions (skip lone declarations like "struct X;")
      if (RD->isThisDeclarationADefinition()) {
        return RD;
      }
    }
  }
  return nullptr;
}

// Configures and uses ASTUnit to compile a file path dynamically into an AST
std::unique_ptr<ASTUnit> parseFileToAST(const std::string &FilePath, 
                                        std::shared_ptr<DiagnosticOptions> DiagOpts, // <-- Pass DiagOpts down
                                        IntrusiveRefCntPtr<DiagnosticsEngine> Diags) {
  
  std::shared_ptr<PCHContainerOperations> PCHOps = std::make_shared<PCHContainerOperations>();
  
  // Set up mock compiler arguments for parsing standard C++17 source files
  std::vector<const char *> Args = {
	  "-Xclang -ast-dump", 
      "-std=c++17",           
      "-fsyntax-only",        
      FilePath.c_str()
  };

  // Clang 21 Signature Fix: Inject both DiagOpts AND Diags as parameters 4 & 5
  std::unique_ptr<ASTUnit> AST = ASTUnit::LoadFromCommandLine(
      Args.data(), Args.data() + Args.size(),
      PCHOps, 
      DiagOpts, // Parameter 4: std::shared_ptr<DiagnosticOptions>
      Diags,    // Parameter 5: IntrusiveRefCntPtr<DiagnosticsEngine>
      ""        // Parameter 6: StringRef ResourceFilesPath
  );

  // Return Wrap Fix: Explicitly wrap with std::move to satisfy GCC 15 toolchain rules
  return std::move(AST);
}


int main(int argc, char *argv[]) {
  // 1. Ensure the user provided two source files to execute a comparison against
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <file1.cpp> <file2.cpp>\n";
    return 1;
  }

  std::string File1 = argv[1];
  std::string File2 = argv[2];

  // 2. Setup the global diagnostic pipeline to capture compilation failures and structural errors
  auto DiagOpts = std::make_shared<DiagnosticOptions>();
  auto DiagClient = std::make_unique<TextDiagnosticPrinter>(llvm::errs(), *DiagOpts);
  IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
  IntrusiveRefCntPtr<DiagnosticsEngine> Diags(
      new DiagnosticsEngine(DiagID, *DiagOpts, DiagClient.release()));

  std::cout << "[Parser] Parsing File 1: " << File1 << "\n";
  auto ASTUnit1 = parseFileToAST(File1, DiagOpts, Diags); // <-- Added DiagOpts

  std::cout << "[Parser] Parsing File 2: " << File2 << "\n";
  auto ASTUnit2 = parseFileToAST(File2, DiagOpts, Diags); // <-- Added DiagOpts


  if (!ASTUnit1 || !ASTUnit2) {
    std::cerr << "[Error] One or both files failed to parse cleanly into Clang's AST.\n";
    return 1;
  }

  // 3. Extract the target Context boundaries from each parsed file layout
  ASTContext &Ctx1 = ASTUnit1->getASTContext();
  ASTContext &Ctx2 = ASTUnit2->getASTContext();

  // 4. Retrieve the comparative top-level structural definitions
  RecordDecl *RD1 = getFirstRecordDecl(Ctx1);
  RecordDecl *RD2 = getFirstRecordDecl(Ctx2);
  
  if (!RD1 || !RD2) {
    std::cerr << "[Error] Could not locate a valid 'struct' or 'class' definition in both files.\n";
    return 1;
  }

  std::cout << "[Structural Matcher] Comparing definitions of '" << RD1->getNameAsString() 
            << "' vs '" << RD2->getNameAsString() << "' with Complain=true...\n\n";

  // 5. Execute structural comparison
  StructuralEquivalenceContext::NonEquivalentDeclSet NonEquivalentDecls;
  StructuralEquivalenceContext StructuralCtx(
      Ctx1.getLangOpts(),
      Ctx1,
      Ctx2,
      NonEquivalentDecls,
      StructuralEquivalenceKind::Default,
      /*StrictTypeSpelling=*/false,
      /*Complain=*/true, // Structural errors emit straight to your TextDiagnosticPrinter
      /*ErrorOnTagTypeMismatch=*/true
  );

  bool equivalent = StructuralCtx.IsEquivalent(RD1, RD2);
  std::cout << "\n[Result] Structural verification finished. Equivalent: " 
            << (equivalent ? "TRUE" : "FALSE") << "\n";

  return 0;
}
