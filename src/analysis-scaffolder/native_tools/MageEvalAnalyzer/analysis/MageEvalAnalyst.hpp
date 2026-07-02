#include <boost/math/distributions/binomial.hpp>
#include <iostream>
#include <unordered_map>
#include "nlohmann/json.hpp"
#include "json.hpp"

using namespace nlohmann;
using namespace std;

/**
 *	Aggregates stats from various sources, then produces probabilities using the aggregated data.
 *	TODO: 
 *		Remove order of processing coupling
 *		Track potential duplication of markers in using the oracle
 *		Get accurate amount of inserted markers from instrumentation origin, e.g. Mage.
 */
class MageEvalAnalyst{
	private:
	public:
	MageEvalAnalyst(){}
	
	/**
	 *	Aggregates stats from the findings data.
	 *	@param	jsonData	Findings data object
	 *	@param	summary		Object for recording/querying results
	 */
	void statisticizeFindings(json& jsonData, json& summary){
		for(auto [markerID, finding] : jsonData.items()){
			incrementJSONEntry(summary["metadata"]["findingsTotal"]);
			if(finding.contains("oracle")){
				incrementJSONEntry(summary["metadata"]["oracleTotal"]);

				auto &oracleData = finding["oracle"][0]; // we cannot assume this to be always present
				auto &nodeOracleStats = summary["constructs"][oracleData["ASTNode"].get<string>()];
				
				incrementJSONEntry(nodeOracleStats["oracleTotal"]);
				statisticizeFindingSpecifics(oracleData["ASTNode"].get<string>(), finding, summary);
				
				auto &recoveredAsMarkee= nodeOracleStats["recoveredAsMarkee"];
				if(finding["decom"].empty()){
					if(nodeOracleStats["viableComparisons"].is_null()){
						nodeOracleStats["viableComparisons"] = 0;
					}
					incrementJSONEntry(nodeOracleStats["absentCorrespondingDecomMarker"]);
					if(recoveredAsMarkee.is_null()){
						recoveredAsMarkee = json::object();
					}
					if(nodeOracleStats["recoveryOfAttr"]["recovered_construct"].is_null()){
						nodeOracleStats["recoveryOfAttr"]["recovered_construct"] = 0;
					}
				} else {
					for(const auto& decomData: finding["decom"]){
						auto &nodeDecomStats = summary["constructs"][decomData["ASTNode"].get<string>()];
						incrementJSONEntry(nodeDecomStats["decomTotal"]);
						incrementJSONEntry(recoveredAsMarkee[decomData["ASTNode"].get<string>()]);
						incrementJSONEntry(summary["metadata"]["decomTotal"]);
						
						incrementJSONEntry(nodeOracleStats["viableComparisons"]);
						incrementJSONEntry(nodeDecomStats["viableComparisons"]);
						incrementJSONEntry(summary["metadata"]["viableComparisons"]);
					}
				}
			} else {
				incrementJSONEntry(summary["metadata"]["oracleMissingTotal"]);
				for(const auto& decomData: finding["decom"]){
					if(summary["constructs"][decomData["ASTNode"].get<string>()]["viableComparisons"].is_null()){
						summary["constructs"][decomData["ASTNode"].get<string>()]["viableComparisons"] = 0;
					}
					if(summary["constructs"][decomData["ASTNode"].get<string>()]["recoveryOfAttr"]["recovered_construct"].is_null()){
						summary["constructs"][decomData["ASTNode"].get<string>()]["recoveryOfAttr"]["recovered_construct"] = 0;
					}
				}
//				summary["metadata"]["oracleMissingTotal"][markerID] = finding["decom"].size(); // useful for diagnostics?
			}
			if(finding["decom"].size() > 1){
				incrementJSONEntry(summary["metadata"]["markerDuplication"]);
			} else if(summary["metadata"]["markerDuplication"].is_null()){
				summary["metadata"]["markerDuplication"] = 0;
			}
		}
	}

	/**
	 *	Aggregates stats from the eval data.
	 *	@param	jsonData	Eval data object
	 *	@param 	summary		Object for recording/querying results
	 */
	void statisticizeEvals(json& jsonData, json& summary){
		for(auto [markerID, evalData] : jsonData.items()){
			for(const auto& evalElement: evalData){
				auto &crrntItem = summary["constructs"][evalElement["markee_construct"]]["recoveryOfAttr"];
				if (crrntItem.is_null()) {crrntItem = json::object();}
				if(evalElement["recovered_construct"].get<bool>()){
					for(auto [key, val]: evalElement.items()){
						if(key == "markee_construct"){continue;} 
						if (crrntItem[key].is_null()){crrntItem[key] = 0;}
						if(val.get<bool>()){incrementJSONEntry(crrntItem[key]);}
					}
					incrementJSONEntry(summary["metadata"]["recoveredConstructsTotal"]);

				} else {
					// Unsuccessful recovery is labeled as deterioration, i.e. recovered_construct == false.
					for(auto [key, val]: evalElement.items()){
						if(key == "markee_construct"){continue;} 
						if (crrntItem[key].is_null()){crrntItem[key] = 0;}
					}
					incrementJSONEntry(summary["metadata"]["deterioratedConstructsTotal"]);
				}
			}
		}
	}

	/**
	 *	Unimplemented method for processing dev information, currently unimplemented.
	 *	@param	jsonData	Dev data object
	 *	@param	summary		Object for recording/querying results
	 */
	void statisticizeDevReports(json& jsonData, json& summary){}
	
	/**
	 *	Map for quick selection of stochastic stmt-processor based on the AST-string.
	 */
	using StochasticPassFnPtr = void(MageEvalAnalyst::*)(json &);
	unordered_map<string, StochasticPassFnPtr> spFnSelector = {
//		{"CallExpr", 		&MageEvalAnalyst::stochasticizeCallExpr},
		{"DeclStmt", 		&MageEvalAnalyst::stochasticizeDeclStmt},
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
			(this->*(it->second))(summary);
        } else {
			// Use general eval, and report/record for devs
		}
	}

	
	void stochasticizeDeclStmt(json& summary){
		// calculate alternative recovery using BinaryOperators of the form "="

	};
	
	/**
	 * 	Sample Data Comparison (Statistical Testing).
	 *	Calculate P and Variance for each constructs separately and in collectively.
	 *	@param summary	Object for recording/querying results
	 */
	void summaryStochasticPass(json& summary){
		for(auto [construct, constructData]: summary["constructs"].items()){
			if(constructData.contains("oracleTotal")){
				if(constructData.contains("recoveryOfAttr") && constructData["viableComparisons"].get<double>() > 0){
					constructData["P(X)"] = constructData["recoveryOfAttr"]["recovered_construct"].get<double>() / constructData["viableComparisons"].get<double>();
					constructData["VAR(X)"] = constructData["P(X)"].get<double>() * (1 - constructData["P(X)"].get<double>());
				} else {
					constructData["P(X)"] = 0;
					constructData["VAR(X)"] = 0;
				}
			} else {
				constructData["P(X)"] = 0;
				constructData["VAR(X)"] = 0;
			}
		}
		if(summary.contains("metadata")){
			summary["metadata"]["P(X)"] 	= summary["metadata"]["recoveredConstructsTotal"].get<double>() / summary["metadata"]["viableComparisons"].get<double>();
			summary["metadata"]["VAR(X)"] 	= summary["metadata"]["P(X)"].get<double>() * (1 - summary["metadata"]["P(X)"].get<double>());
		}
		
		
	}
	
	/**
	 *	Map for quick selection of required stmt-processor based on the AST-string.
	 */
	using FindingsAnalysisFnPtr = void(MageEvalAnalyst::*)(json &, json &);
	unordered_map<string, FindingsAnalysisFnPtr> faFnSelector = {
//		{"CallExpr", 		&MageEvalAnalyst::statisticizeCallExpr},
		{"DeclStmt", 		&MageEvalAnalyst::statisticizeDeclStmt},
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
			(this->*(it->second))(finding, summary);
        } else {
			// Use general eval, and report/record for devs
			statisticizeStmtGeneral(finding, summary);
		}
	}
	
	/**
	 * 	A specific statistication method for DeclStmt.
	 *	@param	finding	A specific finding related to some markerID (Required: oracle node) 
	 *	@param 	summary	Object for recording/querying results
	 */
	void statisticizeDeclStmt(json& finding, json& summary){
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
	void statisticizeStmtGeneral(json& finding, json& summary){}
};
