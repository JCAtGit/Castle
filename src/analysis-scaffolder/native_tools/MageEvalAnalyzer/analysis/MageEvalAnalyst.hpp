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
	 *
	 *
	 */
	void statisticizeFindings(json& jsonData, json& summary){
		cout << "statisticizeFindings" << "\n";
		for(auto [markerID, data] : jsonData.items()){
			auto& oracleData = data["oracle"][0]; // we assume this to be always present
			
			auto &nodeOracleStats = summary["constructs"][oracleData["ASTNode"].get<string>()];
			if(nodeOracleStats["oracleTotal"].is_null()){
				nodeOracleStats["oracleTotal"] = 1;
			} else {
				nodeOracleStats["oracleTotal"] = nodeOracleStats["oracleTotal"].get<int>() + 1;
			}
			
			
			auto &recoveredAsMarkee= summary["constructs"][oracleData["ASTNode"].get<string>()]["recoveredAsMarkee"];
			for(const auto& decomData: data["decom"]){
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
	}

	void statisticizeEvals(json& jsonData, json& summary){
		cout << "statisticizeEvals" << "\n";
		for(auto [markerID, data] : jsonData.items()){
			for(const auto& evalElement: data){
				if(evalElement["recovered_construct"].get<bool>()){
					
//					if(summary["constructs"][evalElement["markee_construct"]]["recoveredTotal"].is_null()){
//						summary["constructs"][evalElement["markee_construct"]]["recoveredTotal"] = 1;
//					} else {
//						summary["constructs"][evalElement["markee_construct"]]["recoveredTotal"] = summary["constructs"][evalElement["markee_construct"]]["recoveredTotal"].get<int>() + 1;
//					}
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