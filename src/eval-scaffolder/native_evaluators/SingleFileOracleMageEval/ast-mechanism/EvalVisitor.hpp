#pragma once

#include <nlohmann/json.hpp>
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace nlohmann;
using namespace clang;

class EvalVisitor: public RecursiveASTVisitor<EvalVisitor> {
	private:
		json& markerFormat;
		SourceManager& SM;
		
		std::unordered_map<std::string, clang::FunctionDecl*> astIndex;
	public:
		EvalVisitor(
			SourceManager& SM, 	json& markerFormat, ASTContext& decomAST) : 
			SM(SM), 			markerFormat(markerFormat)
			{
				indexAST(decomAST);
			}
		
		// Build index from AST: map qualified name -> FunctionDecl*
	    void indexAST(clang::ASTContext &context) {
	        for (auto *D : context.getTranslationUnitDecl()->decls()) {
	            if (auto *FD = llvm::dyn_cast<clang::FunctionDecl>(D)) {
	                std::string name = FD->getQualifiedNameAsString();
	                astIndex[name] = FD;
	            }
	        }
	    }
			
		// Evaluates by traversing through the oracle and then mapping onto the target.
		bool VisitStmt(Stmt *S) {
			if (
				SM.isInMainFile(S->getBeginLoc()) 			&& 
				S->getStmtClass() == Stmt::CallExprClass
			){
				CallExpr* CE = cast<CallExpr>(S);
				auto DC = CE->getDirectCallee();
				if(DC == nullptr){return true;}
				
				// TODO:... 
			}

			// if marker is visited in oracle or decom then establish mapping
			llvm::outs() << S->getStmtClassName() << " test \n";
			return true;
		}
	
	//			bool VisitType(Type *T){return true;}
	//			bool VisitAttr(Attr *A){return true;}
};
