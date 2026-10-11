//#include <iostream>
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
		json 	dataJSON  	 = json::object(), // Container structure with data from: findings, eval and optionally dev.
				summaryJSON  = json::object(),
				metadataJSON = json::object();
	public:
	MageEvalAnalyst(){}
	
	/**
	 *	Aggregates stats from the findings data.
	 *
	 *	@param	jsonData	Findings data object
	 *	@param	summary		Object for recording/querying results
	 */
	void statisticizeFindings(json& dataJSON, json& summaryJSON){
		for(auto [markerID, finding] : dataJSON.items()){
			incrementJSONEntry(summaryJSON["metadata"]["findingsTotal"]);
			if(finding.contains("oracle")){
				incrementJSONEntry(summaryJSON["metadata"]["oracleTotal"]);

				auto &oracleData = finding["oracle"][0]; // we assume this to be always findable, since we insert these ourselves
				auto &nodeOracleStats = summaryJSON["constructs"][oracleData["ASTNode"].get<string>()];
				
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
						auto &nodeDecomStats = summaryJSON["constructs"][decomData["ASTNode"].get<string>()];
						// update the counts
						incrementJSONEntry(nodeDecomStats["decomTotal"]);
						incrementJSONEntry(nodeOracleStats["viableComparisons"]);
						incrementJSONEntry(summaryJSON["metadata"]["decomTotal"]);
						incrementJSONEntry(summaryJSON["metadata"]["viableComparisons"]);
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
				incrementJSONEntry(summaryJSON["metadata"]["oracleMissingTotal"]);
				for(const auto& decomData: finding["decom"]){
					if(summaryJSON["constructs"][decomData["ASTNode"].get<string>()]["viableComparisons"].is_null()){
						summaryJSON["constructs"][decomData["ASTNode"].get<string>()]["viableComparisons"] = 0;
					}
					if(summaryJSON["constructs"][decomData["ASTNode"].get<string>()]["recoveryOfAttr"]["recovered_construct"].is_null()){
						summaryJSON["constructs"][decomData["ASTNode"].get<string>()]["recoveryOfAttr"]["recovered_construct"] = 0;
					}
				}
			}
			
			// TODO: Requires some tweaking, currently functions per translation unit. But does not account for duplications for the same codebase across translation units.
//			if(finding["decom"].size() > 1){
//				incrementJSONEntry(summaryJSON["metadata"]["markerDuplication"]);
//			} else if(summaryJSON["metadata"]["markerDuplication"].is_null()){
//				summaryJSON["metadata"]["markerDuplication"] = 0;
//			}
		}
	}

	/**
	 *	Aggregates stats from the eval data.
	 *
	 *	@param	jsonData	Eval data object
	 *	@param 	summary		Object for recording/querying results
	 */
	void statisticizeEvals(json& dataJSON, json& summaryJSON){
		for(auto [markerID, evalData] : dataJSON.items()){
			for(const auto& evalElement: evalData){
				auto &crrntItem = summaryJSON["constructs"][evalElement["markee_construct"]]["recoveryOfAttr"];
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
					incrementJSONEntry(summaryJSON["metadata"]["recoveredConstructsTotal"]);
				} else {
					// Unsuccessful recovery is labeled as deterioration, i.e. recovered_construct == false.
					// Recording deterioration stats, skipping the name.
					for(auto [key, val]: evalElement.items()){
						if(key == "markee_construct"){continue;} 
						if (crrntItem[key].is_null()){crrntItem[key] = 0;}
					}
					incrementJSONEntry(summaryJSON["metadata"]["deterioratedConstructsTotal"]);
				}
			}
		}
	}

	/**
	 *	Unimplemented method for processing dev information.
	 *
	 *	@param	jsonData	Dev data object
	 *	@param	summary		Object for recording/querying results
	 */
	void statisticizeDevReports(json& dataJSON, json& summaryJSON){}
	
	/**
	 * 	Sample Data Comparison (Statistical Testing).
	 *	Calculate P and Variance for each constructs separately and in collectively.
	 * 	Assumes bernouilli distribution with some value for P, hence variance = p*(p-1).
	 *	Where p, is the decompiler algorithm recovers some criteria correctly or not, e.g. language construct reconstruction.
	 *
	 *	@param summary	Object for recording/querying results
	 */
	void summaryStochasticPass(json& summaryJSON){
		for(auto [construct, constructData]: summaryJSON["constructs"].items()){
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
		if(summaryJSON.contains("metadata")){
			summaryJSON["metadata"]["P(X)"] 	= summaryJSON["metadata"]["recoveredConstructsTotal"].get<double>() / summaryJSON["metadata"]["viableComparisons"].get<double>();
			summaryJSON["metadata"]["VAR(X)"] 	= summaryJSON["metadata"]["P(X)"].get<double>() * (1 - summaryJSON["metadata"]["P(X)"].get<double>());
		}
	}
};

