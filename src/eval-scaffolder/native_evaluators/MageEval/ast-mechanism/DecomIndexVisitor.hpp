#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace nlohmann;
using namespace clang;
using namespace std;

class DecomIndexVisitor: public RecursiveASTVisitor<DecomIndexVisitor> {
	private:
		json &markerFormat; 
		ASTContext &decomAST;		
		unordered_map<string, FunctionDecl*> &decomASTFnDeclIndex; // can be used to check marker integrity amongst other
		unordered_map<int, CallExpr*> &decomASTCallExprIndex; 			// used for quick fetching of markers
		
		// optimizing TODO's
//		IdentifierTable &Idents;
//		IdentifierInfo *TargetII;
	public:
		DecomIndexVisitor(
			json &markerFormat,
			ASTContext &decomAST, 
			unordered_map<string, FunctionDecl*> &decomASTFnDeclIndex, 
			unordered_map<int, CallExpr*> &decomASTCallExprIndex
		) : 
			markerFormat(markerFormat), decomAST(decomAST), decomASTFnDeclIndex(decomASTFnDeclIndex), decomASTCallExprIndex(decomASTCallExprIndex)
			{
				cout << "Indexing decom AST elements" << endl;
//				Idents = decomAST.Idents;
//				TargetII = &Idents.get(markerFormat["name"].get<std::string>());
				indexASTFunctionDecls(decomASTFnDeclIndex, decomAST);
				TraverseDecl(decomAST.getTranslationUnitDecl());
			}
			
			// Build index from AST: map qualified name -> FunctionDecl*
		    void indexASTFunctionDecls(unordered_map<string, clang::FunctionDecl*> &astFnDeclIndex, clang::ASTContext &context) {
		        for (auto *D : context.getTranslationUnitDecl()->decls()) {
		            if (auto *FD = llvm::dyn_cast<clang::FunctionDecl>(D)) {
		                string name = FD->getQualifiedNameAsString();
		                astFnDeclIndex[name] = FD;
		            }
		        }
		    }
		
			
			bool VisitStmt(Stmt *S) {
				// if marker is visited in oracle or decom then establish mapping
//				llvm::outs() << S->getStmtClassName() << " \n";
				return true;
			}
			
//			void indexASTCallExprs(unordered_map<string, clang::FunctionDecl*> &astCallExprIndex, clang::ASTContext &context) {}
			bool VisitCallExpr(CallExpr *CE) { // indexes marker callexpressions by index, which we assume to be unique.
				cout << "visitCE" << endl;
				auto DC = CE->getDirectCallee();
				if(DC == nullptr){return true;}
				if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return true;} // TODO?: check comment at markerformat declaration.
				auto *IL = dyn_cast<IntegerLiteral>(CE->getArg(0)->IgnoreImpCasts());
				if (!IL) {return true;}
				llvm::APInt Value = IL->getValue();
			    int64_t intValue = Value.getZExtValue(); // or getZExtValue() for unsigned
				decomASTCallExprIndex[intValue] = CE;
		       	return true;
			}
	};
	
