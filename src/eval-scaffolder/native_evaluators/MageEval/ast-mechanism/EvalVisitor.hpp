#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"
#include "ast-helper.hpp"

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
		std::unordered_map<std::string, FunctionDecl*> &decomASTFnDeclIndex; 	// can be used to check marker integrity amongst other
		std::unordered_map<int, CallExpr*> &decomASTCallExprIndex; 				// used for quick fetching of markers

		
	public:
		EvalVisitor(
			ASTContext& oracleAST, 
			ASTContext& decomAST,	
			json& markerFormat, 		
			json& evalResults,
			std::unordered_map<std::string, FunctionDecl*> &decomASTFnDeclIndex,
			std::unordered_map<int, CallExpr*> &decomASTCallExprIndex
		) : 
			oracleAST(oracleAST),	
			decomAST(decomAST), 	
			markerFormat(markerFormat),	
			evalResults(evalResults),
			decomASTFnDeclIndex(decomASTFnDeclIndex),
			decomASTCallExprIndex(decomASTCallExprIndex),
			
			oracleSM(oracleAST.getSourceManager()), decomSM(decomAST.getSourceManager())
			{}
		
		//			bool VisitType(Type *T){return true;}
		//			bool VisitAttr(Attr *A){return true;}
			
		// Evaluates by traversing through the oracle and then mapping onto the target.
		bool VisitStmt(Stmt *S) {
			if (oracleSM.isInMainFile(S->getBeginLoc()) && S->getStmtClass() == Stmt::CallExprClass){
				CallExpr* oracleCE = cast<CallExpr>(S);
				auto DC = oracleCE->getDirectCallee();
				if(DC == nullptr){return true;}
				if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return true;} // TODO?: check comment at markerformat declaration.
				// TODO:... 
				json eval = startEval(oracleCE);
			}
			return true;
		}
	
		/**
		 * If a macro is marked, then different procedures need to be taken. Currently macro's are avoided.
		 */
		json startEval(CallExpr* oracleCE){
			json evalResult;
			auto *IL = dyn_cast<IntegerLiteral>(oracleCE->getArg(0)->IgnoreImpCasts());
			if (!IL) {return evalResult;}
			llvm::APInt Value = IL->getValue();
		    int64_t intValue = Value.getZExtValue();
			cout << oracleCE->getDirectCallee()->getNameInfo().getName().getAsString() << ":" << intValue << endl;
			
			auto parents = oracleAST.getParents(*oracleCE);
		   llvm::errs() << "Number of parents: " << parents.size() << "\n";
		   if (parents.size() > 0) {
		       if (const Stmt *parentStmt = parents[0].get<Stmt>()) {
		           llvm::errs() << "Parent Stmt class: " << parentStmt->getStmtClassName() << "\n";
		       } else {
		           llvm::errs() << "Parent is not a Stmt\n";
		       }
		   } else {
		       llvm::errs() << "No parents found – ParentMapContext not built\n";
		   }
			
			optional<DynTypedNode> markedSibling = getNextSibling(DynTypedNode::create(*oracleCE) , oracleAST); // assume not nullopt for oracle
			cout << "check 0" <<endl;

		  	const Stmt *markedSiblingStmt = markedSibling->get<Stmt>();
			cout << "check 1" <<endl;
			switch (markedSiblingStmt->getStmtClass()) {
				case Stmt::CallExprClass: processCallExpr(oracleCE, markedSiblingStmt); break;
				case Stmt::WhileStmtClass: break;
				case Stmt::LabelStmtClass: break;
				case Stmt::VAArgExprClass: break;            // va_arg macro (C89/C99)
				case Stmt::UnaryOperatorClass: break;
				case Stmt::UnaryExprOrTypeTraitExprClass: break;   // sizeof, alignof (C11)
				case Stmt::StringLiteralClass: break;
				case Stmt::PredefinedExprClass: break;        // __func__, __FUNCTION__ (C99)
				case Stmt::ParenExprClass: break;
				case Stmt::OffsetOfExprClass: break;          // offsetof macro (C89)
				case Stmt::MemberExprClass: break;            // structure/union member access
				case Stmt::IntegerLiteralClass: break;
				case Stmt::InitListExprClass: break;          // initialiser lists, compound literals
				case Stmt::ImaginaryLiteralClass: break;      // C99 complex imaginary constants
				case Stmt::GenericSelectionExprClass: break;  // C11 _Generic
				case Stmt::DesignatedInitExprClass: break;    // C99 designated initialisers
				case Stmt::DeclRefExprClass: break;
				case Stmt::CompoundLiteralExprClass: break;   // C99 compound literals
				case Stmt::CharacterLiteralClass: break;
				case Stmt::ImplicitCastExprClass: break;      // implicit conversions (internal, but standard)
				case Stmt::CStyleCastExprClass: break;        // C cast (type)
				case Stmt::BinaryOperatorClass: break;
				case Stmt::CompoundAssignOperatorClass: break;
				case Stmt::ArraySubscriptExprClass: break;
				case Stmt::ConditionalOperatorClass: break;   // ?:
				case Stmt::SwitchStmtClass: break;
				case Stmt::DefaultStmtClass: break;
				case Stmt::CaseStmtClass: break;
				case Stmt::ReturnStmtClass: break;
				case Stmt::NullStmtClass: break;              // ;
				case Stmt::IfStmtClass: break;
				case Stmt::GotoStmtClass: break;
				case Stmt::ForStmtClass: break;
				case Stmt::DoStmtClass: break;
				case Stmt::DeclStmtClass: break;
				case Stmt::ContinueStmtClass: break;
				case Stmt::CompoundStmtClass: break;
				case Stmt::BreakStmtClass: break;
				default: break;
				}
				return evalResult;
			}
			
		void processCallExpr(CallExpr *oracleCE, const Stmt *markedSiblingStmt){
				
		}
			
};
		
