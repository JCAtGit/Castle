
#pragma once

#include <filesystem>
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
	CrossASTEvaluator(
		json &findings,
		json &eval,
		json &dev_report,
		filesystem::path &outputPath
	): findings(findings), eval(eval), dev_report(dev_report), outputPath(outputPath)
	{
		dev_report["Missed"] = json::array();
	}
	

	using EvalFnPtr = bool(CrossASTEvaluator::*)(const string &, json &, json &);
	unordered_map<string, EvalFnPtr> fnSelector = {
//		{"CallExpr", &CrossASTEvaluator::processCallExpr},
		{"DeclStmt", &CrossASTEvaluator::processDeclStmt},
		{"VarDecl", &CrossASTEvaluator::processVarDecl}
	};
	
	void startEval(){
		// key markerID
		for(auto [markerID, finding] : findings.items()){
			evalTraverse(markerID, finding["oracle"], finding["decom"]);
		}
	}
	
	void evalTraverse(const string &markerID, json &findingsOracle, json &findingsDecom){
		auto it = fnSelector.find(findingsOracle["ASTNode"].get<string>());
		bool ct;
	   	if (it != fnSelector.end()) {
            ct = (this->*(it->second))(markerID, findingsOracle, findingsDecom);
        } else {
			// report and record
			eval[markerID][findingsOracle["ASTNode"].get<string>()] = "TODO";
			dev_report["ASTNode"][findingsOracle["ASTNode"].get<string>()] = "TODO";
			dev_report["Missed"].push_back(markerID);
			return;
		}
		if(ct){
			for(size_t i = 0; i < findingsOracle["children"].size(); ++i){
				json decomChild = (i < findingsDecom["children"].size()) ? findingsDecom["children"][i] : json();
				evalTraverse(markerID, findingsOracle["children"][i], decomChild);
			}
		}
	}
	
	private:
	json &findings;
	json &eval;
	json &dev_report;
	filesystem::path &outputPath;
	
	bool processCallExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else {
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processDeclStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else {
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processVarDecl(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else {
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processIfStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else if(findingsDecom["ASTNode"] == "SwitchStmt"){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Overlap";
			return true;
		} else {
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processForStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else if(findingsDecom["ASTNode"] == "WhileStmt"){
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Overlap";
			return true;
		} else {
			eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processImplicitCastExpr(){
		return false;
	}
};

