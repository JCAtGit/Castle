#include <unordered_map>
#include <string>
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "nlohmann/json.hpp"

using namespace clang;
using namespace clang::ast_matchers;
using namespace nlohmann;

class DecomIndexHandler : public MatchFinder::MatchCallback {
public:
    DecomIndexHandler(
        json &markerFormat,
        std::unordered_map<std::string, FunctionDecl*> &fnDeclIndex,
        std::unordered_map<int, CallExpr*> &callExprIndex
    ) : markerFormat(markerFormat), fnDeclIndex(fnDeclIndex), callExprIndex(callExprIndex) {}

    void run(const MatchFinder::MatchResult &Result) override {
		if (const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("call")) {
		    // Now, getParents should work.
		    auto parents = Result.Context->getParents(*CE);
		    if (!parents.empty()) {
		        llvm::outs() << "Parent found!\n";
		        // You can safely traverse parents here to find siblings.
		    } else {
		        llvm::outs() << "No parent found for CallExpr.\n";
		    }
		}
        // Handle FunctionDecl matches (for indexing)
        if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
            std::string name = FD->getQualifiedNameAsString();
            fnDeclIndex[name] = const_cast<FunctionDecl*>(FD);
        }
        
        // Handle CallExpr matches (for marker calls)
        if (const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("call")) {
            const FunctionDecl *DC = CE->getDirectCallee();
            if (!DC) return;
            
            // Check name matches marker function
            std::string targetName = markerFormat["name"].get<std::string>();
            if (DC->getNameInfo().getName().getAsString() != targetName) return;
            
            // Must have exactly one argument
            if (CE->getNumArgs() != 1) return;
            
            // Evaluate the integer argument (handles literals, constants, etc.)
            const Expr *arg = CE->getArg(0)->IgnoreImpCasts();
            Expr::EvalResult evalResult;
            if (arg->EvaluateAsInt(evalResult, *Result.Context)) {
                int64_t intValue = evalResult.Val.getInt().getZExtValue();
                // Store non-const pointer (the AST is alive as long as ASTUnit)
                callExprIndex[intValue] = const_cast<CallExpr*>(CE);
            }
        }
    }

private:
    json &markerFormat;
    std::unordered_map<std::string, FunctionDecl*> &fnDeclIndex;
    std::unordered_map<int, CallExpr*> &callExprIndex;
};