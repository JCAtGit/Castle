#pragma once

#include "string"
#include "nlohmann/json.hpp"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

	void stochasticizeDeclStmt(json& summary){
		// calculate alternative recovery using BinaryOperators of the form "="
	};

	/**
	 *	Map for quick selection of stochastic stmt-processor based on the AST-string.
	 */
	using StochasticPassFnPtr = void(*)(json &);
	unordered_map<string, StochasticPassFnPtr> spFnSelector = {
//		{"CallExpr", 		&MageEvalAnalyst::stochasticizeCallExpr},
//		{"DeclStmt", 		stochasticizeDeclStmt},
//		{"IfStmt", 			&MageEvalAnalyst::stochasticizeIfStmt},
//		{"ForStmt", 		&MageEvalAnalyst::stochasticizeForStmt},
//		{"WhileStmt", 		&MageEvalAnalyst::stochasticizeWhileStmt},
//		{"DoStmt", 			&MageEvalAnalyst::stochasticizeDoStmt},
//		{"BinaryOperator", 	&MageEvalAnalyst::stochasticizeBinaryOperator},
//		{"SwitchStmt", 		&MageEvalAnalyst::stochasticizeSwitchStmt}
	};
	
	void stochasticizeFindingSpecifics(string ASTNode, json& finding, json& summary){
		auto it = spFnSelector.find(ASTNode);
		json markerFindingsResult = json::object();
	   	if (it != spFnSelector.end()) {
			(*(it->second))(summary);
        } else {
			// Use general eval, and report/record for devs
		}
	}
