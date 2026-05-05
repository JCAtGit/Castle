#pragma once

#include "../CodeMarkerInserter.hpp"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;

class CodeMarkerVisitor: public RecursiveASTVisitor<CodeMarkerVisitor> {
	private:
		CodeMarkerInserter& Inserter;
	public:
		explicit CodeMarkerVisitor(CodeMarkerInserter &Inserter) :Inserter(Inserter){}
		
//		bool VisitDecl(Decl *D) {
//			Inserter.processDecl(D);
//			return true;
//		}
	
		bool VisitStmt(Stmt *S) {
			Inserter.processStmt(S);
			return true;
		}
	
	//			bool VisitType(Type *T){return true;}
	//			bool VisitAttr(Attr *A){return true;}
};
