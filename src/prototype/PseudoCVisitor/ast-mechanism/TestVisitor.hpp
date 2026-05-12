#pragma once

#include <nlohmann/json.hpp>
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace nlohmann;
using namespace clang;

class TestVisitor: public RecursiveASTVisitor<TestVisitor> {
	private:
		SourceManager& SM;
	public:
		explicit TestVisitor(SourceManager& SM) : 
			SM(SM)
			{}
			
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
	};