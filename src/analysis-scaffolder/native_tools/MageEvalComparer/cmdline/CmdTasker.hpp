#include <cmath>
#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "helper.hpp"
#include "../compare/MageEvalComparer.hpp"
//#include "matrixAlt.hpp"
#include "matrix.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		std::vector<filesystem::path> &inputDirs;
		filesystem::path &outputPath;				// use outputPath as dir to retrieve previously existing aggregated data from.
		
		string 	&centralizeResultsDir;
		bool 	&centralizeResults;
		
		string dataPath	= "data.json";
		Matrix<json> aggregatedData;

	public:
	CmdTasker(
		std::vector<filesystem::path> &inputDirs, 
		filesystem::path &outputPath,
		string &centralizeResultsDir,
		bool &centralizeResults
	) : 
	inputDirs(inputDirs), 
	outputPath(outputPath),
	centralizeResultsDir(centralizeResultsDir),
	centralizeResults(centralizeResults),
	aggregatedData(inputDirs.size(), inputDirs.size())
	{
		
	}
	
	void loadJSON(filesystem::path jsonFilePath, json& jsonRef){
		ifstream jsonFile(jsonFilePath);
		if (!jsonFile.is_open()) {
		    	cerr << "Error: Could not open required marker formats file, see the source json folder for details." << endl;
		   		return;
		}
	   	try {
			jsonRef = json::parse(jsonFile);
//			cout << jsonRef.dump(2) << "\n";
	   	} catch (const json::parse_error& e) {
	    	cerr << "MageEvalComparer parse error: " << e.what() << endl;
	    	return;
	  	}
	}
	
	void saveJsonToFile(const std::string& path, const json& data) {
	    ofstream file(path);
	    if (!file.is_open()) {
	        std::cerr << "Error: Could not open file for writing: " << path << "\n";
	        return;
	    }
	    file << data.dump(2);  // pretty print with indentation
//	    std::cout << "Saved to "<<  path << "\n" << data.dump(2) << "\n";
	}
	
	void startProcesses(){
		sort(inputDirs.begin(),inputDirs.end());
		std::vector<filesystem::path> currentChoices;
		json iData, jData;
		

		double ZTestResults[inputDirs.size()][inputDirs.size()];
		unordered_map<string, Matrix<double>*> resultsMatrixStore;
		
		for(int i=0 ; i<inputDirs.size() ; i++){
			loadJSON(inputDirs[i], iData);
			currentChoices.push_back(inputDirs[i]);

			for(int j = i + 1 ; j<inputDirs.size() ; j++){				
				loadJSON(inputDirs[j], jData);
				currentChoices.push_back(inputDirs[j]);
				
				aggregatedData.at(i, j) = json::object();
				calcConstructZTestProportions(iData, jData, aggregatedData.at(i, j));
				
				// calc Z-test for proportions, as a whole and if applicable for each construct when enough samples are present
				
				currentChoices.pop_back();
			}
		}
	}
	
	void calcConstructZTestProportions(json& iData, json& jData, json& resultStore){
		auto jConstructs = jData["constructs"];
		for(auto [constructKey, iConstructData]:iData["constructs"].items()){
			if(jConstructs.contains(constructKey)){
				auto jConstructData = jConstructs[constructKey];

				// calculate success/failure condition
				if(iConstructData.contains("oracleTotal") && jConstructData.contains("oracleTotal")){
					auto niOracle = iConstructData["oracleTotal"].get<double>();
					auto piOracle = iConstructData["P[X]"].get<double>();

					auto njOracle = jConstructData["oracleTotal"].get<double>();
					auto pjOracle = jConstructData["P[X]"].get<double>();

					// success/failure condition expectation test for 3 sds, allows normal approximation to a binomial distribution
					// ensure both are eligible for proportion test
					if(
						(
							niOracle * piOracle 		< 10 ||
							niOracle * (1 - piOracle)	< 10
						) ||
						(
							njOracle * pjOracle 		< 10 ||
							njOracle * (1 - pjOracle) 	< 10
						)
					){
						continue;
					}
					// De sample proportions zijn in dit geval hetzelfde als P[X], dus alleen pooled proportions uitrekenen.
					auto pooledProportion = (
						(
							iConstructData["recoveryOfAttr"]["recovered_construct"].get<double>() + 
							jConstructData["recoveryOfAttr"]["recovered_construct"].get<double>()
						) / (
							niOracle + njOracle
						)
					);
					// calc Z statistic, for large disproportions between case 1 and 2 use unpooled (Wald) standard error instead of pooled?
					// we have to make/choose certain assumptions, which validate the approach, i.e. pool vs unpooled. 
					// Do we assume the null hypothesis is true? Or do we remain reserved without assuming H0?
					// 		H0: p_i  = p_j
					//		Ha: p_i != p_j, alternative hypothesis
					// If we assume null is true, then we would be encouraged to use the pooled standard error (SE)
					// If we group the results by the same decompiler, we assume H0, that is the decompiler is the main determinant for found distribution.
					auto pooledSE = sqrt(
						pooledProportion * (1-pooledProportion) *
						(
							(1 / niOracle) +
							(1 / njOracle)
						)
					); 
					auto ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
					resultStore[constructKey] = ZStat2Proportions;
				}
			}
		}
	}
	
	void calcGeneralZTestProportions(json& iData, json& jData, json& resultStore){
		auto niOracle = iData["metadata"]["oracleTotal"].get<double>();
		auto piOracle = iData["metadata"]["P[X]"].get<double>();

		auto njOracle = jData["metadata"]["oracleTotal"].get<double>();
		auto pjOracle = jData["metadata"]["P[X]"].get<double>();
	}
	
};
	