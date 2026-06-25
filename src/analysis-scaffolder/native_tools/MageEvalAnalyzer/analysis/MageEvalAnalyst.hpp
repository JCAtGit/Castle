#include <boost/math/distributions/binomial.hpp>
#include <iostream>
#include <unordered_map>
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

/**
 *	TODO: 
 *		Remove order of processing coupling
 *		Track potential duplication of markers in using the oracle
 *		Get accurate amount of inserted markers from instrumentation origin, e.g. Mage.
 * 	Methods require a certain order of processing:
 *		-
 */
class MageEvalAnalyst{
	private:
		void incrementJSONEntry(json& entry){
			if(entry.is_null()){
				entry = 1;
			} else {
				entry = entry.get<int>() + 1;
			}
		}
		
	public:
	
	MageEvalAnalyst()
	{	
	}
	
	/**
	 * 	TODO:
	 *		Total general recovery
	 *		Test with rufus and then test git
	 *		add lost oracle markers total
	 *		add specific processors
	 *		calculate odds, deviation per stmt for general ASTNode recovery
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
				}
				
//				summary["metadata"]["oracleMissingTotal"][markerID] = finding["decom"].size(); // useful for diagnostics?
			}
			
			if(finding["decom"].size() > 1){
				incrementJSONEntry(summary["metadata"]["markerDuplication"]);
			}
		}
	}

	void statisticizeEvals(json& jsonData, json& summary){
		for(auto [markerID, evalData] : jsonData.items()){
			for(const auto& evalElement: evalData){
				auto &crrntItem = summary["constructs"][evalElement["markee_construct"]]["recoveryOfAttr"];
				if(evalElement["recovered_construct"].get<bool>()){
					incrementJSONEntry(summary["metadata"]["recoveredConstructsTotal"]);
					if (crrntItem.is_null()) {crrntItem = json::object();}
					for(auto [key, val]: evalElement.items()){
						if(key == "markee_construct"){continue;} 
						if(val.get<bool>()){
							incrementJSONEntry(crrntItem[key]);
						} else if (crrntItem[key].is_null()){
							crrntItem[key] = 0;
						}
					}
					
				} else {
					// Unsuccessful recovery is labeled as deterioration, i.e. recovered_construct == false.
					// TODO: set recoverOfAttr to zero for each attr regardless of any found?
					if(crrntItem.is_null()){
						for(auto [key, val]: evalElement.items()){
							if(key == "markee_construct"){continue;} 
							crrntItem[key] = 0;
						}
					}
					incrementJSONEntry(summary["metadata"]["deterioratedConstructsTotal"]);
				}
			}
		}
	}

	void statisticizeDevReports(json& jsonData, json& summary){
		
	}
	
	/**
	 * 	Sample Data Comparison (Statistical Testing).
	 */
	void summaryStochasticPass(json& summary){
		// calculate p and variance for all constructs and in general
		for(auto [construct, constructData]: summary["constructs"].items()){
			if(constructData.contains("oracleTotal")){
				if(constructData.contains("recoveryOfAttr")){
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
		// final metadata stats like P(X) and VAR(X).
		if(summary.contains("metadata")){
			summary["metadata"]["P(X)"] 	= summary["metadata"]["recoveredConstructsTotal"].get<double>() / summary["metadata"]["viableComparisons"].get<double>();
			summary["metadata"]["VAR(X)"] 	= summary["metadata"]["P(X)"].get<double>() * (1 - summary["metadata"]["P(X)"].get<double>());
		}
	}
	
	using FindingsAnalysisFnPtr = void(MageEvalAnalyst::*)(json &, json &);
	unordered_map<string, FindingsAnalysisFnPtr> fnSelector = {
//		{"CallExpr", 		&MageEvalAnalyst::statisticizeCallExpr},
		{"DeclStmt", 		&MageEvalAnalyst::statisticizeDeclStmt},
//		{"IfStmt", 			&MageEvalAnalyst::statisticizeIfStmt},
//		{"ForStmt", 		&MageEvalAnalyst::statisticizeForStmt},
//		{"WhileStmt", 		&MageEvalAnalyst::statisticizeWhileStmt},
//		{"DoStmt", 			&MageEvalAnalyst::statisticizeDoStmt},
//		{"BinaryOperator", 	&MageEvalAnalyst::statisticizeBinaryOperator},
//		{"SwitchStmt", 		&MageEvalAnalyst::statisticizeSwitchStmt}
	};
	
	void statisticizeFindingSpecifics(string ASTNode, json& finding, json& summary){
		auto it = fnSelector.find(ASTNode);
		json markerFindingsResult = json::object();
	   	if (it != fnSelector.end()) {
			(this->*(it->second))(finding, summary);
        } else {
			// Use general eval, and report/record for devs
			statisticizeStmtGeneral(finding, summary);
		}
	}
	
	/**
	 * 	Requires oracle presence
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
	
	void statisticizeStmtGeneral(json& finding, json& summary){
			
	}
};