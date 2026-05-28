#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTUnit.h"

using namespace clang;
using namespace clang::tooling;

// Shared data: both ASTs
struct ASTPair {
    std::unique_ptr<ASTUnit> UnitA;
    std::unique_ptr<ASTUnit> UnitB;
} GlobalASTs;

// Visitor for AST A that can access AST B
class CrossASTVisitor : public RecursiveASTVisitor<CrossASTVisitor> {
public:
    CrossASTVisitor(ASTContext *ContextA, ASTContext *ContextB)
        : ContextA(ContextA), ContextB(ContextB) {}

    bool VisitFunctionDecl(FunctionDecl *FD) {
        // Example: find a function with the same name in AST B
        if (!FD->getIdentifier()) return true;
        std::string Name = FD->getNameAsString();

        // Look up the function in AST B by name
        DeclarationName DName = &ContextB->Idents.get(Name);
        DeclContext *DC = ContextB->getTranslationUnitDecl();
        for (auto *Decl : DC->lookup(DName)) {
            if (auto *FD_B = dyn_cast<FunctionDecl>(Decl)) {
                // We found the matching function in file B
                // Now we can do something with it, e.g., compare signatures
                if (FD_B->getReturnType() == FD->getReturnType()) {
                    // ...
                }
            }
        }
        return true;
    }

private:
    ASTContext *ContextA;
    ASTContext *ContextB;
};

int main(int argc, char **argv) {
    if (argc < 3) return 1;

    std::string FileA = argv[1];
    std::string FileB = argv[2];

    // Common compiler arguments
    std::vector<std::string> Args = {
        "-xc++", "-std=c++11", "-I/usr/include"
    };

    // Build ASTUnit for file A
    GlobalASTs.UnitA = buildASTFromCodeWithArgs(
        FileA, Args, "fileA.cpp");

    // Build ASTUnit for file B
    GlobalASTs.UnitB = buildASTFromCodeWithArgs(
        FileB, Args, "fileB.cpp");

    if (!GlobalASTs.UnitA || !GlobalASTs.UnitB) {
        llvm::errs() << "Failed to parse files\n";
        return 1;
    }

    // Get ASTContexts
    ASTContext *ContextA = &GlobalASTs.UnitA->getASTContext();
    ASTContext *ContextB = &GlobalASTs.UnitB->getASTContext();

    // Traverse AST A, with access to AST B
    CrossASTVisitor Visitor(ContextA, ContextB);
    Visitor.TraverseDecl(ContextA->getTranslationUnitDecl());

    return 0;
}
