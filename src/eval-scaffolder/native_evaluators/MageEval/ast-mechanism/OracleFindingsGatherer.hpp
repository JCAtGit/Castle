#pragma once

#include <unordered_map>
#include <string>
#include "clang/AST/ASTTypeTraits.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "ast-helper.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace nlohmann;

class OracleFindingsGatherer: public MatchFinder::MatchCallback {
	public:
		OracleFindingsGatherer(json& markerFormat, ASTContext& oracleAST) : markerFormat(markerFormat), oracleAST(oracleAST){}
		
		void run(const MatchFinder::MatchResult &result) override {
			const CallExpr *markerCE = result.Nodes.getNodeAs<CallExpr>("ofgCallExpr");
			auto DC = markerCE->getDirectCallee();
			if(DC == nullptr){return;}
			cout << DC->getNameInfo().getName().getAsString() << " : " << markerFormat["name"].get<string>() << endl;
			if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return;} // TODO?: check comment at markerformat declaration.
			
			auto *IL = dyn_cast<IntegerLiteral>(markerCE->getArg(0)->IgnoreImpCasts());
			if (!IL) {return;}
			llvm::APInt Value = IL->getValue();
		    int64_t intValue = Value.getZExtValue();
			cout << markerCE->getDirectCallee()->getNameInfo().getName().getAsString() << " : " << intValue << endl;
			
			auto parents = oracleAST.getParents(*markerCE);
			auto parentStmt = parents[0].get<Stmt>();
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
			auto siblings = getChildrenNodes(parentStmt);
			auto markerCESiblingIndex = findNodeSiblingIndex(siblings, DynTypedNode::create(*markerCE));
		}

	private:
		json& markerFormat;
		ASTContext& oracleAST;
		
		void gatherMarkerMetadata(CallExpr *markerCE, llvm::SmallVector<DynTypedNode, 16> siblings, int markerCESiblingIndex){
			
		}
		
		void selectProcessor(Stmt* tgtStmt){
			switch (tgtStmt->getStmtClass()){
				case Stmt::CallExprClass:  break;
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
		}

		void processDeclStmt(){
			
		}
		
};
