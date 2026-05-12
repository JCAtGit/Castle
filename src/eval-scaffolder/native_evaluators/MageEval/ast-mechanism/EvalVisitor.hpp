#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace nlohmann;
using namespace clang;
using namespace std;

class EvalVisitor: public RecursiveASTVisitor<EvalVisitor> {
	private:
		json& markerFormat;			// assumption: compiled code retains marker declaration name info because of noopt, but decompilers remain wildcard.
		SourceManager& oracleSM;
		
		std::unordered_map<std::string, clang::FunctionDecl*> astIndex;
	public:
		EvalVisitor(
			SourceManager& oracleSM, 	json& markerFormat, 		ASTContext& decomAST) : 
			oracleSM(oracleSM), 		markerFormat(markerFormat)
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
			if (oracleSM.isInMainFile(S->getBeginLoc()) && S->getStmtClass() == Stmt::CallExprClass){
				CallExpr* CE = cast<CallExpr>(S);
				auto DC = CE->getDirectCallee();
				if(DC == nullptr){return true;}
				if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return true;} // TODO?: check comment at markerformat declaration.
				if (auto *IL = dyn_cast<IntegerLiteral>(CE->getArg(0))) {
				    llvm::APInt Value = IL->getValue();
				    int64_t intValue = Value.getSExtValue(); // or getZExtValue() for unsigned
					cout << DC->getNameInfo().getName().getAsString() << intValue <<  endl;
				}
				// TODO:... 
				
			}

			return true;
		}
	
	//			bool VisitType(Type *T){return true;}
	//			bool VisitAttr(Attr *A){return true;}
};
