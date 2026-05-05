#pragma once

#include <nlohmann/json.hpp>
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/Support/raw_ostream.h"

using namespace nlohmann;
using namespace clang;

class EvalVisitor: public RecursiveASTVisitor<EvalVisitor> {
	private:
		json& markerFormat;
		
	public:
		explicit EvalVisitor(json& markerFormat) : markerFormat(markerFormat){}
		
//		bool VisitDecl(Decl *D) {
//			return true;
//		}
	
		bool VisitStmt(Stmt *S) {
			llvm::outs() << S->getStmtClassName() << " test \n";
			return true;
		}
	
	//			bool VisitType(Type *T){return true;}
	//			bool VisitAttr(Attr *A){return true;}
};
