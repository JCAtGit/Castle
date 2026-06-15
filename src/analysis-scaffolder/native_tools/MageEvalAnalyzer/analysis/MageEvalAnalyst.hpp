#include <boost/math/distributions/binomial.hpp>
#include "nlohmann/json.hpp"

using namespace nlohmann;

class MageEvalAnalyst{
	private:
	
	
	
	
	public:
	
	MageEvalAnalyst()
	{
			
	}
	
	
	void startAnalysis(){
		
	}
	
	void statisticizeFindings(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
			auto oracleData = data["oracle"][0]; // we assume this to be always present
			
		}
	}

	void statisticizeEvals(json& jsonData, json& summary){
		for(auto [markerID, data] : jsonData.items()){
					
		}
	}

	void statisticizeDevReports(json& jsonData, json& summary){
		
	}
};