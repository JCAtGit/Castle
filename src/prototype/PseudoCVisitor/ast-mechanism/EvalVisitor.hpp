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
		SourceManager& oracleSM;
	public:
		explicit EvalVisitor(SourceManager& SM) : oracleSM(SM){}
		
		// Evaluates by traversing through the oracle and then mapping onto the target.
		bool VisitStmt(Stmt *S) {
			if (
				oracleSM.isInMainFile(S->getBeginLoc()) 			&& 
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
