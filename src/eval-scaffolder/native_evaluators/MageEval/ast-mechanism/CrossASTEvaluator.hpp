
#pragma once

#include <cstddef>
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

#include "clang/Frontend/TextDiagnosticPrinter.h"
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
		json &findings_eval,
		json &dev_report,
		filesystem::path &outputPath
	): findings(findings), findings_eval(findings_eval), dev_report(dev_report), outputPath(outputPath)
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
		auto markerFindings = findings.items();
		for(size_t i = 0; i<findings.size(); i++){
			
		}
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
			findings_eval[markerID][findingsOracle["ASTNode"].get<string>()] = "TODO";
			dev_report["ASTNode"][findingsOracle["ASTNode"].get<string>()] = "TODO";
			dev_report["Unevaluated"].push_back(markerID);
			return;
		}
		if(!ct){return;}
		for(size_t i = 0; i < findingsOracle["children"].size(); ++i){
			json decomChild = (i < findingsDecom["children"].size()) ? findingsDecom["children"][i] : json();
			evalTraverse(markerID, findingsOracle["children"][i], decomChild);
		}
	}
	
	private:
	json &findings;
	json &findings_eval;
	json &dev_report;
	filesystem::path &outputPath;
	
	bool processCallExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else {
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processDeclStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else {
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processVarDecl(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else {
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processIfStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else if(findingsDecom["ASTNode"] == "SwitchStmt"){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Overlap";
			return true;
		} else {
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processForStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else if(findingsDecom["ASTNode"] == "WhileStmt"){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Overlap";
			return true;
		} else {
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processWhileStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		if(findingsDecom["ASTNode"] == findingsOracle["ASTNode"]){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Present";
			return true;
		} else if(findingsDecom["ASTNode"] == "ForStmt"){
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Overlap";
			return true;
		} else {
			findings_eval[markerID][findingsOracle.at("ASTNode")]["has_construct"] = "Absent";
			return false;
		}
	}
	
	bool processDoStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		return false;
	}
	
	bool processImplicitCastExpr(){
		return false;
	}
	
	bool processRecoveryExpr(){
		return false;
	}
};

