#pragma once

#include <unordered_map>
#include <string>
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "ast-helper.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace nlohmann;

class DecomFindingsGatherer: public MatchFinder::MatchCallback {
	public:
		DecomFindingsGatherer(json& markerFormat, ASTContext& decomAST): markerFormat(markerFormat), decomAST(decomAST){}
		
		void run(const MatchFinder::MatchResult &result) override {
			const CallExpr *markerCE = result.Nodes.getNodeAs<CallExpr>("dfgCallExpr");
			auto DC = markerCE->getDirectCallee();
			if(DC == nullptr){return;}
			if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return;} // TODO?: check comment at markerformat declaration.
			
//			optional<DynTypedNode> markedSibling = getNextSibling(DynTypedNode::create(*markerCE) , decomAST); // assume not nullopt for oracle
			auto parents = decomAST.getParents(*markerCE);
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
		}

	private:
	json& markerFormat;
	ASTContext& decomAST;
};
