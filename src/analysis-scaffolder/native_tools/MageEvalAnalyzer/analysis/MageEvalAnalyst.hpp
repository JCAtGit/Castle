#include <boost/math/distributions/binomial.hpp>
#include <iostream>
#include <unordered_map>
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

/**
 *	TODO: Remove order of processing coupling
 * 	Methods require a certain order of processing:
 *		-
 */
class MageEvalAnalyst{
	private:
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
			if(summary["metadata"]["oracleTotal"].is_null()){
				summary["metadata"]["oracleTotal"] = 1;
			} else {
				summary["metadata"]["oracleTotal"] = summary["metadata"]["oracleTotal"].get<int>() + 1;
			}
			
			if(finding.contains("oracle")){
				auto &oracleData = finding["oracle"][0]; // we cannot assume this to be always present
				auto &nodeOracleStats = summary["constructs"][oracleData["ASTNode"].get<string>()];
				if(nodeOracleStats["oracleTotal"].is_null()){
					nodeOracleStats["oracleTotal"] = 1;
				} else {
					nodeOracleStats["oracleTotal"] = nodeOracleStats["oracleTotal"].get<int>() + 1;
				}
				
				statisticizeFindingSpecifics(oracleData["ASTNode"].get<string>(), finding, summary);
				
				auto &recoveredAsMarkee= nodeOracleStats["recoveredAsMarkee"];
				if(finding["decom"].empty()){
					if(nodeOracleStats["absentCorrespondingDecomMarker"].is_null()){
						nodeOracleStats["absentCorrespondingDecomMarker"] = 1;
					} else {
						nodeOracleStats["absentCorrespondingDecomMarker"] = nodeOracleStats["absentCorrespondingDecomMarker"].get<int>() + 1;
					}
				} else {
					for(const auto& decomData: finding["decom"]){
						
						if(summary["metadata"]["decomTotal"].is_null()){
							summary["metadata"]["decomTotal"] = 1;
						} else {
							summary["metadata"]["decomTotal"] = summary["metadata"]["decomTotal"].get<int>() + 1;
						}
						
						auto &nodeDecomStats = summary["constructs"][decomData["ASTNode"].get<string>()];
						if(nodeDecomStats["decomTotal"].is_null()){
							nodeDecomStats["decomTotal"] = 1;
						} else {
							nodeDecomStats["decomTotal"] = nodeDecomStats["decomTotal"].get<int>() + 1;
						}
						
						// general recovery stats
						if(recoveredAsMarkee[decomData["ASTNode"].get<string>()].is_null()){
							recoveredAsMarkee[decomData["ASTNode"].get<string>()] = 1;
						} else {
							recoveredAsMarkee[decomData["ASTNode"].get<string>()] = recoveredAsMarkee[decomData["ASTNode"].get<string>()].get<int>() + 1;	
						}
					}
				}
			} else {
				summary["metadata"]["lostOracleMarkers"][markerID] = finding["decom"].size();
			}
		}
	}

	void statisticizeEvals(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
			for(const auto& evalElement: data){
				if(evalElement["recovered_construct"].get<bool>()){
					if(summary["metadata"]["recoveredConstructsTotal"].is_null()){
						summary["metadata"]["recoveredConstructsTotal"] = 1;
					} else {
						summary["metadata"]["recoveredConstructsTotal"] = summary["metadata"].value("recoveredConstructsTotal", 1) + 1;
					}
					
					auto &crrntItem = summary["constructs"][evalElement["markee_construct"]]["recoveryOfAttr"];
					for(auto [key, val]: evalElement.items()){
						if (crrntItem.is_null()) {
						    crrntItem = json::object();  // or simply = {};
						}
						if(key == "markee_construct"){
							
						} else if(val.get<bool>()){
							crrntItem[key] = crrntItem.value(key, 0) + 1;
						}
					}
				}
			}
		}
	}

	void statisticizeDevReports(json& jsonData, json& summary){
		
	}
	
	// Sample Data Comparison (Statistical Testing)
	void summaryStochasticPass(json& summary){
		// calculate p and variance for all constructs and in general
		for(auto [construct, constructData]: summary["constructs"].items()){
			if(constructData.contains("oracleTotal")){
				if(constructData.contains("recoveryOfAttr")){
					constructData["P(X)"] = constructData["recoveryOfAttr"]["recovered_construct"].get<double>() / constructData["oracleTotal"].get<double>();
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
		// final metadata stats like P(X) and VAR(X)
		if(summary.contains("metadata")){
			summary["metadata"]["P(X)"] 	= summary["metadata"]["recoveredConstructsTotal"].get<double>() / summary["metadata"]["oracleTotal"].get<double>();
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
			if(typeStats[typeInfo["type"]].is_null()){
				typeStats[typeInfo["type"]] = 1;
			} else {
				typeStats[typeInfo["type"]] = typeStats.value(typeInfo["type"], 1) + 1;
			}
		}		
	}
	
	void statisticizeStmtGeneral(json& finding, json& summary){
			
	}
};