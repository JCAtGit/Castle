#pragma once

#include "string"
#include "nlohmann/json.hpp"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

/**
 * 	A specific statistication method for DeclStmt.
 *	@param	finding	A specific finding related to some markerID (Required: oracle node) 
 *	@param 	summary	Object for recording/querying results
 */
void statisticizeFindingDeclStmt(json& finding, json& summary){
	// record declstmt types frequency
	auto &oracleData = finding["oracle"][0];
	auto &nodeOracleStats = summary["constructs"][oracleData["ASTNode"].get<string>()];
	
	auto &typeStats = nodeOracleStats["oracleMarkedTypes"];
	for(auto typeInfo : oracleData["properties"]["init_types"]){
		incrementJSONEntry(typeStats[typeInfo["type"]]);
	}
}

/**
 *	General method for unspecified procession.
 *	TODO:
 *		Dev reports or otherwise other useful methods can be implemented here
 *	@param	finding	A specific finding related to some markerID
 *	@param	summary	Object for recording/querying results
 */
void statisticizeFindingStmtGeneral(json& finding, json& summary){}

	/**
	 *	Map for quick selection of required stmt-processor based on the AST-string.
	 */
	using FindingsAnalysisFnPtr = void (*)(json &, json &);
	unordered_map<string, FindingsAnalysisFnPtr> faFnSelector = {
//		{"CallExpr", 		&MageEvalAnalyst::statisticizeCallExpr},
		{"DeclStmt", 		statisticizeFindingDeclStmt},
//		{"IfStmt", 			&MageEvalAnalyst::statisticizeIfStmt},
//		{"ForStmt", 		&MageEvalAnalyst::statisticizeForStmt},
//		{"WhileStmt", 		&MageEvalAnalyst::statisticizeWhileStmt},
//		{"DoStmt", 			&MageEvalAnalyst::statisticizeDoStmt},
//		{"BinaryOperator", 	&MageEvalAnalyst::statisticizeBinaryOperator},
//		{"SwitchStmt", 		&MageEvalAnalyst::statisticizeSwitchStmt}
	};
	
	/**
	 *	Applies a specific statistication method or a general one.
	 *	@param ASTNode	String designation of selected ASTNode
	 *	@param finding	A specific finding related to some markerID
	 *	@param summary	Object for recording/querying results
	 */
	void statisticizeFindingSpecifics(string ASTNode, json& finding, json& summary){
		auto it = faFnSelector.find(ASTNode);
		json markerFindingsResult = json::object();
	   	if (it != faFnSelector.end()) {
			(*(it->second))(finding, summary);
        } else {
			// Use general eval, and report/record for devs
			statisticizeFindingStmtGeneral(finding, summary);
		}
	}
	