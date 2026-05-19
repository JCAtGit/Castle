#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTStructuralEquivalence.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "llvm/TargetParser/Host.h"
#include <iostream>
#include <memory>
using namespace clang;

// Struct to act as an RAII container preventing managers from going out of scope prematurely
struct CompleteASTEnvironment {
  std::unique_ptr<FileManager> FileMgr;
  std::unique_ptr<SourceManager> SourceMgr;
  std::unique_ptr<IdentifierTable> Idents;
  std::unique_ptr<SelectorTable> Sels;
  std::unique_ptr<Builtin::Context> Builtins;
  std::shared_ptr<TargetInfo> Target;
  std::unique_ptr<ASTContext> Context;
};

// Fixed Factory function that safely registers target architectures and prevents null pointer crashes
std::unique_ptr<CompleteASTEnvironment> createASTEnvironment(
    LangOptions &LangOpts,
    std::shared_ptr<TargetOptions> TargetOpts,
    IntrusiveRefCntPtr<DiagnosticsEngine> Diags) {

  auto Env = std::make_unique<CompleteASTEnvironment>();
  
  auto FileOpts = FileSystemOptions();
  Env->FileMgr = std::make_unique<FileManager>(FileOpts);
  Env->SourceMgr = std::make_unique<SourceManager>(*Diags, *Env->FileMgr);
  Env->Builtins = std::make_unique<Builtin::Context>();
  Env->Idents = std::make_unique<IdentifierTable>(LangOpts);
  Env->Sels = std::make_unique<SelectorTable>();

  // Allocate target infrastructure
  Env->Target = std::shared_ptr<TargetInfo>(TargetInfo::CreateTargetInfo(*Diags, *TargetOpts));

  // Initialize the 6-argument Clang 21 context
  Env->Context = std::make_unique<ASTContext>(
      LangOpts, 
      *Env->SourceMgr, 
      *Env->Idents, 
      *Env->Sels, 
      *Env->Builtins, 
      TranslationUnitKind::TU_Complete
  );

  // CRITICAL FIX: Manually link the target info layout and initialize underlying types
  Env->Context->InitBuiltinTypes(*Env->Target);

  return Env;
}

// Helper to quickly generate a test RecordDecl (struct) containing one typed field
RecordDecl *createTestRecord(ASTContext &Context, StringRef StructName, 
                             StringRef FieldName, QualType FieldType) {
  
  TranslationUnitDecl *TUD = Context.getTranslationUnitDecl();
  
  RecordDecl *RD = RecordDecl::Create(
      Context, TagTypeKind::Struct, TUD, 
      SourceLocation(), SourceLocation(), &Context.Idents.get(StructName));
  RD->startDefinition();

  FieldDecl *FD = FieldDecl::Create(
      Context, RD, SourceLocation(), SourceLocation(), 
      &Context.Idents.get(FieldName), FieldType, nullptr, 
      /*BitWidth=*/nullptr, /*Mutable=*/false, InClassInitStyle::ICIS_NoInit);
  
  FD->setAccess(AccessSpecifier::AS_public);
  RD->addDecl(FD);
  RD->completeDefinition();

  return RD;
}

int main() {
  LangOptions LangOpts;
  LangOpts.CPlusPlus = true; 

  auto DiagOpts = std::make_shared<DiagnosticOptions>();
  auto DiagClient = std::make_unique<TextDiagnosticPrinter>(llvm::errs(), *DiagOpts);
  IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
  IntrusiveRefCntPtr<DiagnosticsEngine> Diags(
      new DiagnosticsEngine(DiagID, *DiagOpts.get(), DiagClient.release()));

  auto TargetOpts = std::make_shared<TargetOptions>();
  TargetOpts->Triple = llvm::sys::getDefaultTargetTriple();

  std::cout << "[Setup] Creating isolated environments...\n";
  auto Env1 = createASTEnvironment(LangOpts, TargetOpts, Diags);
  auto Env2 = createASTEnvironment(LangOpts, TargetOpts, Diags);

  std::cout << "[Setup] Creating structurally differing fields...\n";
  // struct Target { int data; }
  RecordDecl *RD1 = createTestRecord(*Env1->Context, "Target", "data", Env1->Context->IntTy);
  // struct Target { float data; } (Type Mismatch!)
  RecordDecl *RD2 = createTestRecord(*Env2->Context, "Target", "data", Env2->Context->FloatTy);

  std::cout << "[Execution] Running StructuralEquivalenceContext with Complain=true...\n\n";

  StructuralEquivalenceContext::NonEquivalentDeclSet NonEquivalentDecls;
  StructuralEquivalenceContext Ctx(
      Env1->Context->getLangOpts(),
      *Env1->Context,
      *Env2->Context,
      NonEquivalentDecls,
      StructuralEquivalenceKind::Default,
      /*StrictTypeSpelling=*/false,
      /*Complain=*/true, 
      /*ErrorOnTagTypeMismatch=*/false
  );

  bool equivalent = Ctx.IsEquivalent(RD1, RD2);

  std::cout << "\n[Result] Verification concluded. Equivalent: " 
            << (equivalent ? "TRUE" : "FALSE") << "\n";

  return 0;
}
