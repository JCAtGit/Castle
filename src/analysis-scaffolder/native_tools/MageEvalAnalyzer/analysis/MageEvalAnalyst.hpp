#include <iostream>
#include <string>
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

				auto &oracleData = finding["oracle"][0]; // we assume this to be always findable, since we insert these ourselves
				auto &nodeOracleStats = summary["constructs"][oracleData["ASTNode"].get<string>()];
				
				incrementJSONEntry(nodeOracleStats["oracleTotal"]);
				auto &recoveredAsMarkee= nodeOracleStats["recoveredAsMarkee"];
				if(finding["decom"].empty()){
					if(nodeOracleStats["viableComparisons"].is_null()){
						nodeOracleStats["viableComparisons"] = 0;
					}
					if(recoveredAsMarkee.is_null()){
						recoveredAsMarkee = json::object();
					}
					if(nodeOracleStats["recoveryOfAttr"]["recovered_construct"].is_null()){
						nodeOracleStats["recoveryOfAttr"]["recovered_construct"] = 0;
					}
				} else {
					for(const auto& decomData: finding["decom"]){
						auto &nodeDecomStats = summary["constructs"][decomData["ASTNode"].get<string>()];
						// update the counts
						incrementJSONEntry(nodeDecomStats["decomTotal"]);
						incrementJSONEntry(nodeOracleStats["viableComparisons"]);
						incrementJSONEntry(summary["metadata"]["decomTotal"]);
						incrementJSONEntry(summary["metadata"]["viableComparisons"]);
						// record decom recovery instance
						incrementJSONEntry(recoveredAsMarkee[decomData["ASTNode"].get<string>()]);

						// record decom node existence  if not known yet
						if(nodeDecomStats["viableComparisons"].is_null()){
							nodeDecomStats["viableComparisons"] = 0;
						}
					}
				}
				// A id was found with decom entries but not an oracle entry
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
				if (crrntItem.is_null()) {
					crrntItem = json::object();
					crrntItem["recovered_construct"] = 0;
				}
				if(evalElement["recovered_construct"].get<bool>()){
					for(auto [key, val]: evalElement.items()){
						if(key == "markee_construct"){continue;} 
						if (crrntItem[key].is_null()){crrntItem[key] = 0;}
						if(val.get<bool>()){incrementJSONEntry(crrntItem[key]);}
					}
					incrementJSONEntry(summary["metadata"]["recoveredConstructsTotal"]);
				} else {
					// Unsuccessful recovery is labeled as deterioration, i.e. recovered_construct == false.
					// Recording deterioration stats, skipping the name.
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
};

