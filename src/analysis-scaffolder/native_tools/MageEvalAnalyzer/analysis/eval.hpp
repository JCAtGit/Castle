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
void statisticizeEvalDeclStmt(const json& eval, json& summary){
	auto &crrntItem = summary["constructs"][eval["markee_construct"]]["recoveryOfAttr"];
	for(auto [key, val]: eval.items()){
		if(key == "markee_construct"){continue;} 
		if (crrntItem[key].is_null()){crrntItem[key] = 0;}
		if(val.get<bool>()){incrementJSONEntry(crrntItem[key]);}
	}
}

/**
 *	General method for unspecified procession.
 *	TODO:
 *		Dev reports or otherwise other useful methods can be implemented here
 *	@param	finding	A specific finding related to some markerID
 *	@param	summary	Object for recording/querying results
 */
void statisticizeEvalStmtGeneral(const json& eval, json& summary){}

	/**
	 *	Map for quick selection of required stmt-processor based on the AST-string.
	 */
	using EvalAnalysisFnPtr = void (*)(const json &, json &);
	unordered_map<string, EvalAnalysisFnPtr> eaFnSelector = {
//		{"CallExpr", 		&MageEvalAnalyst::statisticizeCallExpr},
		{"DeclStmt", 		statisticizeEvalDeclStmt},
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
	void statisticizeEvalSpecifics(string ASTNode, const json& eval, json& summary){
		auto it = eaFnSelector.find(ASTNode);
		json markerFindingsResult = json::object();
	   	if (it != eaFnSelector.end()) {
			(*(it->second))(eval, summary);
        } else {
			// Use general eval, and report/record for devs
			statisticizeEvalStmtGeneral(eval, summary);
		}
	}
	