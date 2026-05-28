
#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTStructuralEquivalence.h"

#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

// assumes oracle to be free of major errors and to be compilable.
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
		{"CallExpr", &CrossASTEvaluator::processCallExpr},
		{"DeclStmt", &CrossASTEvaluator::processDeclStmt},
		{"IfStmt", &CrossASTEvaluator::processIfStmt},
		{"ForStmt", &CrossASTEvaluator::processForStmt},
		{"WhileStmt", &CrossASTEvaluator::processWhileStmt},
		{"DoStmt", &CrossASTEvaluator::processDoStmt},
		{"BinaryOperator", &CrossASTEvaluator::processBinaryOperator}
//		{"VarDecl", &CrossASTEvaluator::processVarDecl}
	};
	
	void startEval(){
		auto markerFindings = findings.items();
//		for(size_t i = 0; i<findings.size(); i++){ // check marker survival?
//			
//		}
		for(auto [markerID, finding] : findings.items()){
			evalTraverse(markerID, finding["oracle"], finding["decom"]);
		}
	}
	
	void evalTraverse(const string &markerID, json &findingsOracle, json &findingsDecom){
		auto it = fnSelector.find(findingsOracle["ASTNode"].get<string>());
		bool ct;
	   	if (it != fnSelector.end()) {// TODO: rework for slight speedup
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
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		
		markerFindingsResult["name_preserved"] = (findingsOracle["properties"]["direct_callee_name"] == findingsDecom["properties"]["direct_callee_name"]);
		markerFindingsResult["parity_params_count"] = (findingsOracle["properties"]["num_params"] == findingsDecom["properties"]["num_params"]);
		markerFindingsResult["parity_args_count"] = (findingsOracle["properties"]["num_args"] == findingsDecom["properties"]["num_args"]);
		markerFindingsResult["_variadicity"] = (findingsOracle["properties"]["isVariadic"] == findingsDecom["properties"]["isVariadic"]);

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processDeclStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"].get<string>();
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));
//		
		findings_eval[markerID] = markerFindingsResult;
		return false;

	}
	
	bool processVarDecl(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;

	}
	
	bool processIfStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;

	}
	
	bool processForStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;

	}
	
	bool processWhileStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;
	}
	
	bool processDoStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;
	}
	
	bool processBinaryOperator(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;
	}
	
	bool processImplicitCastExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;
	}
	
	bool processRecoveryExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && findingsDecom["ASTNode"] == findingsOracle["ASTNode"]);
		return false;
	}
};

