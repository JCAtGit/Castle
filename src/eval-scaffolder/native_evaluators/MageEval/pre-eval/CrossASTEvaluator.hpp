
#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTStructuralEquivalence.h"

#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/raw_ostream.h"
#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class CrossASTEvaluator{
	public:
	CrossASTEvaluator(json &findings): findings(findings){}
	

	using EvalFnPtr = void(CrossASTEvaluator::*)(const string &, json &);
	unordered_map<string, EvalFnPtr> fnSelector = {
		{"CallExpr", &CrossASTEvaluator::processCallExpr},
		{"DeclStmt", &CrossASTEvaluator::processDeclStmt},
		{"VarDecl", &CrossASTEvaluator::processVarDecl}
	};
		
	void startEval(){
		for(auto [key, val] : findings.items()){
			auto it = fnSelector.find(val["ASTNode"].get<std::string>());
            if (it != fnSelector.end()) {
                (this->*(it->second))(key, val);
            } else {
				// report and record
			}
		}
	}
	private:
	json &findings;
	
	void processCallExpr(const string &, json &){}
	void processDeclStmt(const string &, json &){}
	void processVarDecl(const string &, json &){}

};

