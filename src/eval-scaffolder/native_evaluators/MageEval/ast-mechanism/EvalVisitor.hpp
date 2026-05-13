#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
//#include "../evaluation-mechanism/selector.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class EvalVisitor: public RecursiveASTVisitor<EvalVisitor> {
	private:
		ASTContext &oracleAST, &decomAST;
		json& markerFormat;			// assumption: compiled code retains marker declaration name info because of noopt, but decompilers remain wildcard.
		json& evalResults;
		SourceManager& oracleSM;
		SourceManager& decomSM;
		std::unordered_map<std::string, clang::FunctionDecl*> decomASTIndex;
		
		// Build index from AST: map qualified name -> FunctionDecl*
	    void indexASTFunctionDecls(unordered_map<string, clang::FunctionDecl*> &astFnDeclIndex, clang::ASTContext &context) {
	        for (auto *D : context.getTranslationUnitDecl()->decls()) {
	            if (auto *FD = llvm::dyn_cast<clang::FunctionDecl>(D)) {
	                string name = FD->getQualifiedNameAsString();
	                astFnDeclIndex[name] = FD;
	            }
	        }
	    }
	public:
		EvalVisitor(
			ASTContext& oracleAST, 	ASTContext& decomAST,	json& markerFormat, 		json& evalResults) : 
			oracleAST(oracleAST),	decomAST(decomAST), 	markerFormat(markerFormat),	evalResults(evalResults),
			
			oracleSM(oracleAST.getSourceManager()), decomSM(decomAST.getSourceManager())
			{
				indexASTFunctionDecls(decomASTIndex, decomAST);
			}
		
		//			bool VisitType(Type *T){return true;}
		//			bool VisitAttr(Attr *A){return true;}
			
		// Evaluates by traversing through the oracle and then mapping onto the target.
		bool VisitStmt(Stmt *S) {
			if (oracleSM.isInMainFile(S->getBeginLoc()) && S->getStmtClass() == Stmt::CallExprClass){
				CallExpr* CE = cast<CallExpr>(S);
				auto DC = CE->getDirectCallee();
				if(DC == nullptr){return true;}
				if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return true;} // TODO?: check comment at markerformat declaration.
//				cout << DC->getNameInfo().getName().getAsString() << endl;
				// TODO:... 
				startEval(CE);
			}
			return true;
		}
	
		/**
		 * If a macro is marked, then different procedures need to be taken. Currently macro's are avoided.
		 */
		void startEval(CallExpr* CE){
			auto *IL = dyn_cast<IntegerLiteral>(CE->getArg(0)->IgnoreImpCasts());
			if (!IL) {return;}
			llvm::APInt Value = IL->getValue();
		    int64_t intValue = Value.getSExtValue(); // or getZExtValue() for unsigned
			cout << "intValue:" << intValue <<  endl;
		}

};
