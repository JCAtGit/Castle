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

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		std::vector<filesystem::path> &inputDirs;
		filesystem::path &outputPath;				// use outputPath as dir to retrieve previously existing aggregated data from.
		
		string 	&centralizeResultsDir;
		bool 	&centralizeResults;
		
		string dataPath		= "data.json";
		json aggregatedData = 		json::object();
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
	centralizeResults(centralizeResults)
	{
		if(filesystem::exists(centralizeResultsDir + dataPath)){
			loadJSON(centralizeResultsDir, aggregatedData);
		}
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
		for(int i=0 ; i<inputDirs.size() ; i++){
			loadJSON(inputDirs[i], iData);
			currentChoices.push_back(inputDirs[i]);

			for(int j = i + 1 ; j<inputDirs.size() ; j++){
				currentChoices.push_back(inputDirs[j]);
				loadJSON(inputDirs[j], jData);

				// haal json bestanden op
				
				// calc Z-test for proportions, as a whole and if applicable for each construct when enough samples are present
				
				currentChoices.pop_back();
			}
		}
	}
	
	void calcZTestProportions(json& iData, json& jData){
		auto jConstructs = jData["constructs"];
		for(auto [constructKey, iConstructData]:iData["constructs"].items()){
			if(jConstructs.contains(constructKey)){
				auto jConstructData = jConstructs[constructKey];

				// calculate success/failure condition
				if(iConstructData.contains("oracleTotal") && jConstructData.contains("oracleTotal")){
					auto niOracle = iConstructData["oracleTotal"].get<int>();
					auto njOracle = jConstructData["oracleTotal"].get<int>();
					// success/failure condition expectation test for 3 sds, allows normal approximation to a binomial distribution
					// ensure both are eligible for proportion test
					if(
						(
							niOracle * iConstructData["P[X]"].get<double>() 	< 10 ||
							niOracle * (1-iConstructData["P[X]"].get<double>())	< 10
						) ||
						(
							njOracle * jConstructData["P[X]"].get<double>() 	< 10 ||
							njOracle * (1-jConstructData["P[X]"].get<double>()) < 10
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
					// calc Z statistic, for large disproportions between case 1 and 2 use Unpooled (Wald) standard error instead of pooled?

//					auto ZStat;
					
					// eval P value

				}

			}
		}
	}
	
};
	