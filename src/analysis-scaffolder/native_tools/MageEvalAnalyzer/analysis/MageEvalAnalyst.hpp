#include <boost/math/distributions/binomial.hpp>
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

class MageEvalAnalyst{
	private:
	
	
	
	
	public:
	
	MageEvalAnalyst()
	{
			
	}
	
	
	void startAnalysis(){
		
	}
	
	/**
	 * 	TODO:
	 *
	 *
	 */
	void statisticizeFindings(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
			auto oracleData = data["oracle"][0]; // we assume this to be always present

			auto &crrntItem = summary["constructs"][oracleData["ASTContext"].get<string>()]["recoveryAsStmts"];
			for(const auto& decomData: data["decom"]){
				// general recovery stats
				crrntItem[decomData["ASTContext"].get<string>()] = crrntItem.value(decomData["ASTContext"].get<string>(), 0) + 1;						
			}
		}
	}

	void statisticizeEvals(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
			for(const auto& evalElement: data){
				if(evalElement["recovered_construct"].get<bool>()){
					auto &crrntItem = summary["construct"][evalElement["markee_construct"]]["recoveryOfAttr"];
					for(auto [key, val]: evalElement.items()){
						if(val){
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