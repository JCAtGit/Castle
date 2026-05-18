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
			
			optional<DynTypedNode> markedSibling = getNextSibling(DynTypedNode::create(*markerCE) , oracleAST); // assume not nullopt for oracle
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
			auto markerCESiblingIndex = findNodeSiblingIndex(siblings, DynTypedNode::create(markerCE));
		}

	private:
		json& markerFormat;
		ASTContext& oracleAST;
		
		void gatherMarkerMetadata(CallExpr *markerCE){
			getPrev
		}
		
		void selectProcessor(){
			
		}

};
