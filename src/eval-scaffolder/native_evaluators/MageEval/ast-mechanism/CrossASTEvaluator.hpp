
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using namespace nlohmann;
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
		dev_report["missing"] = json::array();
		dev_report["incomplete"] = json::array();
		dev_report["unevaluated"] = json::array();
	}
	
	using EvalFnPtr = bool(CrossASTEvaluator::*)(const string &, json &, json &);
	unordered_map<string, EvalFnPtr> fnSelector = {
		{"CallExpr", &CrossASTEvaluator::processCallExpr},
		{"DeclStmt", &CrossASTEvaluator::processDeclStmt},
		{"IfStmt", &CrossASTEvaluator::processIfStmt},
		{"ForStmt", &CrossASTEvaluator::processForStmt},
		{"WhileStmt", &CrossASTEvaluator::processWhileStmt},
		{"DoStmt", &CrossASTEvaluator::processDoStmt},
		{"BinaryOperator", &CrossASTEvaluator::processBinaryOperator},
		{"SwitchStmt", &CrossASTEvaluator::processSwitchStmt}
	};
	
	void startEval(){
		auto markerFindings = findings.items();
		for(size_t i = 0; i<findings.size(); i++){ // check marker survival?
			if(!findings.contains(std::to_string(i))){
				dev_report["missing"].push_back(i);
			}
		}
		for(auto [markerID, finding] : findings.items()){
			// TODO: Create dummy fail object, to punish the absence of decom instances.
			if(finding.contains("oracle") && finding.contains("decom")){ 
				evalTraverse(markerID, finding["oracle"], finding["decom"]);
			} else {
				dev_report["incomplete"].push_back(markerID);
			}
		}
	}
	
	void evalTraverse(const string &markerID, json &findingsOracle, json &findingsDecom){
		auto it = fnSelector.find(findingsOracle["ASTNode"].get<string>());
	   	if (it != fnSelector.end()) {
            (this->*(it->second))(markerID, findingsOracle, findingsDecom);
        } else {
			// Use general eval, and report/record for devs
			dev_report["generic_eval"].push_back(markerID);
			processStmtGeneral(markerID, findingsOracle, findingsDecom);
		}
	}
	
	private:
	json &findings;
	json &findings_eval;
	json &dev_report;
	filesystem::path &outputPath;
	
	bool processStmtGeneral(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));
		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processCallExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));
		
		if(markerFindingsResult["recovered_construct"]){
			if(findingsDecom["properties"]["has_direct_callee"]){
				markerFindingsResult["name_preserved"] = (findingsOracle["properties"]["direct_callee_name"] == findingsDecom["properties"]["direct_callee_name"]);
				markerFindingsResult["parity_params_count"] = (findingsOracle["properties"]["num_params"] == findingsDecom["properties"]["num_params"]);
				sort(findingsOracle["properties"]["param_types"].begin(),findingsOracle["properties"]["param_types"].end());
				sort(findingsDecom["properties"]["param_types"].begin(),findingsDecom["properties"]["param_types"].end());
				markerFindingsResult["parity_param_types"] = findingsOracle["properties"]["param_types"] == findingsDecom["properties"]["param_types"];
				markerFindingsResult["parity_has_direct_callee"] = findingsOracle["properties"]["has_direct_callee"] == findingsDecom["properties"]["has_direct_callee"];
			} else {
				markerFindingsResult["name_preserved"] = false;
				markerFindingsResult["parity_params_count"] = false;
				markerFindingsResult["parity_param_types"] = false;
			}
			// TODO: type check kan misschien nog fijndradiger,
			
			markerFindingsResult["parity_args_count"] = (findingsOracle["properties"]["num_args"] == findingsDecom["properties"]["num_args"]);
			sort(findingsOracle["properties"]["arg_types"].begin(),findingsOracle["properties"]["arg_types"].end());
			sort(findingsDecom["properties"]["arg_types"].begin(),findingsDecom["properties"]["arg_types"].end());
			markerFindingsResult["parity_arg_types"] = findingsOracle["properties"]["arg_types"] == findingsDecom["properties"]["arg_types"];

			markerFindingsResult["parity_variadicity"] = (findingsOracle["properties"]["is_variadic"] == findingsDecom["properties"]["is_variadic"]);
			markerFindingsResult["parity_side_effects"] = (findingsOracle["properties"]["has_side_effects"] == findingsDecom["properties"]["has_side_effects"]);
			markerFindingsResult["parity_has_direct_callee"] = (findingsOracle["properties"]["has_direct_callee"] == findingsDecom["properties"]["has_direct_callee"]);
			
			findings_eval[markerID] = markerFindingsResult;
			return false;
		}
		// TODO, direct callee is niet vanzelfsprekend maar args kunnen nog afgeleid worden
		markerFindingsResult["name_preserved"] = false;
		markerFindingsResult["parity_params_count"] = false;
		markerFindingsResult["parity_param_types"] = false;
		markerFindingsResult["parity_args_count"] = false;
		markerFindingsResult["parity_arg_types"] = false;
		markerFindingsResult["parity_variadicity"] = false;
		markerFindingsResult["parity_side_effects"] = false;
		markerFindingsResult["parity_has_direct_callee"] = false;

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processDeclStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));
		
		if(markerFindingsResult["recovered_construct"]){
			markerFindingsResult["parity_single_decl"] = (findingsOracle["properties"]["single_decl"] == findingsDecom["properties"]["single_decl"]);
			
			sort(findingsOracle["properties"]["init_types"].begin(), findingsOracle["properties"]["init_types"].end());
			sort(findingsDecom["properties"]["init_types"].begin(), findingsDecom["properties"]["init_types"].end());

			markerFindingsResult["parity_init_types"] = findingsOracle["properties"]["init_types"] == findingsDecom["properties"]["init_types"];
			return false;
		}
		
		markerFindingsResult["parity_single_decl"] = false;
		markerFindingsResult["parity_init_types"] = false;
		
		findings_eval[markerID] = markerFindingsResult;
		return false;

	}

	bool processIfStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));
		
		if(markerFindingsResult["recovered_construct"]){
			markerFindingsResult["parity_cond_expr_types"] = findingsOracle["properties"]["cond_expr_types"] == findingsDecom["properties"]["cond_expr_types"];
			markerFindingsResult["parity_cond_stmt_class"] = findingsOracle["properties"]["cond_stmt_class"] == findingsDecom["properties"]["cond_stmt_class"];
		} else {
			markerFindingsResult["parity_cond_stmt_class"] = false;
		}
		
		findings_eval[markerID] = markerFindingsResult;
		return false;

	}
	
	bool processForStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));

		if(markerFindingsResult["recovered_construct"]){
			// init test
			if(findingsOracle["properties"]["has_init"] && findingsDecom["properties"]["has_init"]){
				markerFindingsResult["parity_has_init"] = true;
				markerFindingsResult["parity_init_stmt_class"] = findingsOracle["properties"]["init_stmt_class"] == findingsDecom["properties"]["init_stmt_class"];
				
				sort(findingsOracle["properties"]["init_types"].begin(), findingsOracle["properties"]["init_types"].end());
				sort(findingsDecom["properties"]["init_types"].begin(), findingsDecom["properties"]["init_types"].end());

				markerFindingsResult["parity_init_types"] = findingsOracle["properties"]["init_types"] == findingsDecom["properties"]["init_types"];
			} else {
				markerFindingsResult["parity_has_init"] = false;
				markerFindingsResult["parity_init_stmt_class"] = false;
				markerFindingsResult["parity_init_types"] = false;
			}
			
			// condvar tests
			if(findingsOracle["properties"]["has_init"] && findingsDecom["properties"]["has_init"]){
				markerFindingsResult["parity_has_cond_var"] = true;
				markerFindingsResult["parity_cond_var_stmt_class"] = findingsOracle["properties"]["cond_var_stmt_class"] == findingsDecom["properties"]["cond_var_stmt_class"];
				
				sort(findingsOracle["properties"]["cond_var_types"].begin(), findingsOracle["properties"]["cond_var_types"].end());
				sort(findingsDecom["properties"]["cond_var_types"].begin(), findingsDecom["properties"]["cond_var_types"].end());
				markerFindingsResult["parity_cond_var_types"] =  findingsOracle["properties"]["cond_var_types"] == findingsDecom["properties"]["cond_var_types"];
			} else {
				markerFindingsResult["parity_has_cond_var"] = false;
				markerFindingsResult["parity_cond_var_stmt_class"] = false;
				markerFindingsResult["parity_cond_var_types"] = false;
			}

			// cond tests
			if(findingsOracle["properties"]["has_cond_expr"] && findingsDecom["properties"]["has_cond_expr"]){
				markerFindingsResult["parity_has_cond_expr"] = true;
				markerFindingsResult["parity_cond_stmt_class"] = findingsOracle["properties"]["cond_stmt_class"] == findingsDecom["properties"]["cond_stmt_class"];

				sort(findingsOracle["properties"]["cond_expr_types"].begin(), findingsOracle["properties"]["cond_expr_types"].end());
				sort(findingsDecom["properties"]["cond_expr_types"].begin(), findingsDecom["properties"]["cond_expr_types"].end());
				markerFindingsResult["parity_cond_expr_types"] = findingsOracle["properties"]["cond_expr_types"] == findingsDecom["properties"]["cond_expr_types"];
			} else {
				markerFindingsResult["parity_has_cond_expr"] = false;
				markerFindingsResult["parity_cond_stmt_class"] = false;
				markerFindingsResult["parity_cond_expr_types"] = false;
			}
			
			// incr test
			if(findingsOracle["properties"]["has_incr"] && findingsDecom["properties"]["has_incr"]){
				markerFindingsResult["parity_has_incr"] = true;
				markerFindingsResult["parity_incr_stmt_class"] = findingsOracle["properties"]["incr_stmt_class"] == findingsDecom["properties"]["incr_stmt_class"];
				
				sort(findingsOracle["properties"]["incr_expr_types"].begin(), findingsOracle["properties"]["incr_expr_types"].end());
				sort(findingsDecom["properties"]["incr_expr_types"].begin(), findingsDecom["properties"]["incr_expr_types"].end());
				markerFindingsResult["parity_incr_expr_types"] = findingsOracle["properties"]["incr_expr_types"] == findingsDecom["properties"]["incr_expr_types"];
			} else {
				markerFindingsResult["parity_has_incr"] = false;
				markerFindingsResult["parity_incr_stmt_class"] = false;
				markerFindingsResult["parity_incr_expr_types"] = false;
			}
		} else {
			markerFindingsResult["parity_has_init"] = false;
			markerFindingsResult["parity_init_stmt_class"] = false;
			markerFindingsResult["parity_init_types"] = false;
			
			markerFindingsResult["parity_has_cond_var"] = false;
			markerFindingsResult["parity_cond_var_stmt_class"] = false;
			markerFindingsResult["parity_cond_var_types"] = false;
			
			markerFindingsResult["parity_has_cond_expr"] = false;
			markerFindingsResult["parity_cond_stmt_class"] = false;
			markerFindingsResult["parity_cond_expr_types"] = false;
			
			markerFindingsResult["parity_has_incr"] = false;
			markerFindingsResult["parity_incr_stmt_class"] = false;
			markerFindingsResult["parity_incr_expr_types"] = false;
		}
		
		findings_eval[markerID] = markerFindingsResult;
		return false;

	}
	
	bool processWhileStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));

		if(markerFindingsResult["recovered_construct"]){
			// condvar tests
			if(findingsOracle["properties"]["has_cond_var"] && findingsDecom["properties"]["has_cond_var"]){
				markerFindingsResult["parity_has_cond_var"] = true;
				markerFindingsResult["parity_condvar_stmt_class"] = findingsOracle["properties"]["condvar_stmt_class"] == findingsDecom["properties"]["condvar_stmt_class"];
				
				sort(findingsOracle["properties"]["cond_var_types"].begin(), findingsOracle["properties"]["cond_var_types"].end());
				sort(findingsDecom["properties"]["cond_var_types"].begin(), findingsDecom["properties"]["cond_var_types"].end());
				markerFindingsResult["parity_cond_var_types"] =  findingsOracle["properties"]["cond_var_types"] == findingsDecom["properties"]["cond_var_types"];
			} else {
				markerFindingsResult["parity_has_cond_var"] = false;
				markerFindingsResult["parity_cond_var_stmt_class"] = false;
				markerFindingsResult["parity_cond_var_types"] = false;
			}

			// cond tests
			if(findingsOracle["properties"]["has_cond_expr"] && findingsDecom["properties"]["has_cond_expr"]){
				markerFindingsResult["parity_has_cond_expr"] = true;
				markerFindingsResult["parity_cond_stmt_class"] = findingsOracle["properties"]["cond_stmt_class"] == findingsDecom["properties"]["cond_stmt_class"];

				sort(findingsOracle["properties"]["cond_expr_types"].begin(), findingsOracle["properties"]["cond_expr_types"].end());
				sort(findingsDecom["properties"]["cond_expr_types"].begin(), findingsDecom["properties"]["cond_expr_types"].end());
				markerFindingsResult["parity_cond_expr_types"] = findingsOracle["properties"]["cond_expr_types"] == findingsDecom["properties"]["cond_expr_types"];
			} else {
				markerFindingsResult["parity_has_cond_expr"] = false;
				markerFindingsResult["parity_cond_stmt_class"] = false;
				markerFindingsResult["parity_cond_expr_types"] = false;
			}
		} else {
			markerFindingsResult["parity_has_cond_var"] = false;
			markerFindingsResult["parity_cond_var_stmt_class"] = false;
			markerFindingsResult["parity_cond_var_types"] = false;
			
			markerFindingsResult["parity_has_cond_expr"] = false;
			markerFindingsResult["parity_cond_stmt_class"] = false;
			markerFindingsResult["parity_cond_expr_types"] = false;
		}

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processDoStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));

		if(markerFindingsResult["recovered_construct"]){
					
		}

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processBinaryOperator(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));

		if(markerFindingsResult["recovered_construct"]){
			markerFindingsResult["parity_operator"] = 	findingsOracle["properties"]["operator"] == findingsDecom["properties"]["operator"];
			sort(findingsOracle["properties"]["operator_types"].begin(), findingsOracle["properties"]["operator_types"].end());
			sort(findingsDecom["properties"]["operator_types"].begin(), findingsDecom["properties"]["operator_types"].end());
			markerFindingsResult["parity_operator_types"] = findingsOracle["properties"]["operator_types"] == findingsDecom["properties"]["operator_types"];
			
			markerFindingsResult["parity_lhs_stmt_class"] = findingsOracle["properties"]["lhs_stmt_class"] == findingsDecom["properties"]["lhs_stmt_class"];
			sort(findingsOracle["properties"]["lhs_types"].begin(), findingsOracle["properties"]["lhs_types"].end());
			sort(findingsDecom["properties"]["lhs_types"].begin(), findingsDecom["properties"]["lhs_types"].end());
			markerFindingsResult["parity_lhs_types"] = findingsOracle["properties"]["lhs_types"] == findingsDecom["properties"]["lhs_types"];
			
			markerFindingsResult["parity_rhs_stmt_class"] = findingsOracle["properties"]["rhs_stmt_class"] == findingsDecom["properties"]["rhs_stmt_class"];
			sort(findingsOracle["properties"]["rhs_types"].begin(), findingsOracle["properties"]["rhs_types"].end());
			sort(findingsDecom["properties"]["rhs_types"].begin(), findingsDecom["properties"]["rhs_types"].end());
			markerFindingsResult["parity_rhs_types"] = findingsOracle["properties"]["rhs_types"] == findingsDecom["properties"]["rhs_types"];

		} else {
			markerFindingsResult["parity_operator"] = false;
			markerFindingsResult["parity_operator_types"] = false;
			
			markerFindingsResult["parity_lhs_stmt_class"] = false;
			markerFindingsResult["parity_lhs_types"] = false;

			markerFindingsResult["parity_rhs_stmt_class"] = false;
			markerFindingsResult["parity_rhs_types"] = false;
		}

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processSwitchStmt(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));

		if(markerFindingsResult["recovered_construct"]){
		// init test
			if(findingsOracle["properties"]["has_init"] && findingsDecom["properties"]["has_init"]){
				markerFindingsResult["parity_has_init"] = true;
				markerFindingsResult["parity_init_stmt_class"] = findingsOracle["properties"]["init_stmt_class"] == findingsDecom["properties"]["init_stmt_class"];
				
				sort(findingsOracle["properties"]["init_types"].begin(), findingsOracle["properties"]["init_types"].end());
				sort(findingsDecom["properties"]["init_types"].begin(), findingsDecom["properties"]["init_types"].end());
	
				markerFindingsResult["parity_init_types"] = findingsOracle["properties"]["init_types"] == findingsDecom["properties"]["init_types"];
			} else {
				markerFindingsResult["parity_has_init"] = false;
				markerFindingsResult["parity_init_stmt_class"] = false;
				markerFindingsResult["parity_init_types"] = false;
			}
			
			// condvar tests
			if(findingsOracle["properties"]["has_init"] && findingsDecom["properties"]["has_init"]){
				markerFindingsResult["parity_has_cond_var"] = true;
				markerFindingsResult["parity_cond_var_stmt_class"] = findingsOracle["properties"]["cond_var_stmt_class"] == findingsDecom["properties"]["cond_var_stmt_class"];
				
				sort(findingsOracle["properties"]["cond_var_types"].begin(), findingsOracle["properties"]["cond_var_types"].end());
				sort(findingsDecom["properties"]["cond_var_types"].begin(), findingsDecom["properties"]["cond_var_types"].end());
				markerFindingsResult["parity_cond_var_types"] =  findingsOracle["properties"]["cond_var_types"] == findingsDecom["properties"]["cond_var_types"];
			} else {
				markerFindingsResult["parity_has_cond_var"] = false;
				markerFindingsResult["parity_cond_var_stmt_class"] = false;
				markerFindingsResult["parity_cond_var_types"] = false;
			}
	
			// cond tests
			if(findingsOracle["properties"]["has_cond_expr"] && findingsDecom["properties"]["has_cond_expr"]){
				markerFindingsResult["parity_has_cond_expr"] = true;
				markerFindingsResult["parity_cond_stmt_class"] = findingsOracle["properties"]["cond_stmt_class"] == findingsDecom["properties"]["cond_stmt_class"];
	
				sort(findingsOracle["properties"]["cond_expr_types"].begin(), findingsOracle["properties"]["cond_expr_types"].end());
				sort(findingsDecom["properties"]["cond_expr_types"].begin(), findingsDecom["properties"]["cond_expr_types"].end());
				markerFindingsResult["parity_cond_expr_types"] = findingsOracle["properties"]["cond_expr_types"] == findingsDecom["properties"]["cond_expr_types"];
			} else {
				markerFindingsResult["parity_has_cond_expr"] = false;
				markerFindingsResult["parity_cond_stmt_class"] = false;
				markerFindingsResult["parity_cond_expr_types"] = false;
			}
			
			} else {
				markerFindingsResult["parity_has_init"] = false;
				markerFindingsResult["parity_init_stmt_class"] = false;
				markerFindingsResult["parity_init_types"] = false;
				
				markerFindingsResult["parity_has_cond_var"] = false;
				markerFindingsResult["parity_cond_var_stmt_class"] = false;
				markerFindingsResult["parity_cond_var_types"] = false;
				
				markerFindingsResult["parity_has_cond_expr"] = false;
				markerFindingsResult["parity_cond_stmt_class"] = false;
				markerFindingsResult["parity_cond_expr_types"] = false;
			}
			// deze info is bugged vanuit clang, dus nog niet zo zinvol
			markerFindingsResult["parity_has_switch_cases"] = findingsOracle["properties"]["has_switch_cases"] == findingsDecom["properties"]["has_switch_cases"];
			markerFindingsResult["parity_switch_cases"] = findingsOracle["properties"]["switch_cases"] == findingsDecom["properties"]["switch_cases"];
			return false;
		}
		
	
	bool processImplicitCastExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));
		// TODO:
//		if(markerFindingsResult["recovered_construct"]){}

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
	
	bool processRecoveryExpr(const string &markerID, json &findingsOracle,json &findingsDecom){
		json markerFindingsResult = json::object();
		markerFindingsResult["markee_construct"] = findingsOracle["ASTNode"];
		markerFindingsResult["recovered_construct"] = (findingsDecom["properties"]["has_markee"] && (findingsDecom["ASTNode"] == findingsOracle["ASTNode"]));

		if(markerFindingsResult["recovered_construct"]){
					
		}

		findings_eval[markerID] = markerFindingsResult;
		return false;
	}
};

