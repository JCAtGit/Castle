#include <boost/math/distributions/binomial.hpp>
#include <iostream>
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

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
	 */
	void statisticizeFindings(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
			if(summary["metadata"]["oracleTotal"].is_null()){
				summary["metadata"]["oracleTotal"] = 1;
			} else {
				summary["metadata"]["oracleTotal"] = summary["metadata"]["oracleTotal"].get<int>() + 1;
			}
			if(data.contains("oracle")){
				auto &oracleData = data["oracle"][0]; // we assume this to be always present
				auto &nodeOracleStats = summary["constructs"][oracleData["ASTNode"].get<string>()];
				if(nodeOracleStats["oracleTotal"].is_null()){
					nodeOracleStats["oracleTotal"] = 1;
				} else {
					nodeOracleStats["oracleTotal"] = nodeOracleStats["oracleTotal"].get<int>() + 1;
				}
				auto &recoveredAsMarkee= summary["constructs"][oracleData["ASTNode"].get<string>()]["recoveredAsMarkee"];
				if(data["decom"].empty()){
					if(nodeOracleStats["absentCorrespondingDecomMarker"].is_null()){
						nodeOracleStats["absentCorrespondingDecomMarker"] = 1;
					} else {
						nodeOracleStats["absentCorrespondingDecomMarker"] = nodeOracleStats["absentCorrespondingDecomMarker"].get<int>() + 1;
					}
				} else {
					for(const auto& decomData: data["decom"]){
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
				summary["metadata"]["lostOracleMarkers"][markerID] = data["decom"].size();
			}
		}
	}

	void statisticizeEvals(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
			for(const auto& evalElement: data){
				if(evalElement["recovered_construct"].get<bool>()){
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
};