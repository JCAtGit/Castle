#pragma once

#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;

class EvalVisitor: public RecursiveASTVisitor<EvalVisitor> {
	private:
	public:
		explicit EvalVisitor(){}
		
//		bool VisitDecl(Decl *D) {
//			return true;
//		}
	
		bool VisitStmt(Stmt *S) {
			return true;
		}
	
	//			bool VisitType(Type *T){return true;}
	//			bool VisitAttr(Attr *A){return true;}
};
