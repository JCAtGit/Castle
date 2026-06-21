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
		const string summaryFilename = "summary.json";
		
		string 	&centralizeResultsDir;
		bool 	&centralizeResults;
		
		unordered_map<string, Matrix<double>> matrixContextMap; // contains contexts general and the constructs from the summaries.
		
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
	

	template <typename T>
	void exportZTestsMatrixToCSV(const std::string& path, Matrix<T>& contextData){
		// export construct and general values to CSV
		string CSVData = "ZScore";
		for(int i = 0; i<inputDirs.size();i++){
			
		}
		
		
		
		CSVData = CSVData + "ZScore";
		for(int i = 0; i<inputDirs.size();i++){
			CSVData = CSVData + "," + inputDirs[i].string();
		}
		CSVData = CSVData + "\n";

		for(int i=0 ; i<inputDirs.size() ; i++){
			CSVData = CSVData + inputDirs[i].string();
			for(int j = 0 ; j<inputDirs.size() ; j++){
				CSVData = CSVData + "," + to_string(contextData.at(i, j));
			}
			CSVData = CSVData + "\n";
		}
		ofstream file(path);
	    if (!file.is_open()) {
	        std::cerr << "Error: Could not open file for writing: " << path << "\n";
	        return;
	    }
	    file << CSVData;  // pretty print with indentation
	}
	
	void startProcesses(){
		cout << "Start processes \n";
		sort(inputDirs.begin(),inputDirs.end());
		std::vector<filesystem::path> currentChoices;
		json iData, jData;
		
		for(int i=0 ; i<inputDirs.size() ; i++){
			loadJSON(inputDirs[i].string() + summaryFilename, iData);
			currentChoices.push_back(inputDirs[i]);
			cout 	<< inputDirs[i].string() + summaryFilename << "\n"
					<< iData.dump(2) 
					<< "\n";

			for(int j = i + 1 ; j<inputDirs.size() ; j++){
				loadJSON(inputDirs[j].string() + summaryFilename, jData);
				currentChoices.push_back(inputDirs[j]);
				cout 	<< inputDirs[j].string() + summaryFilename << "\n"
						<< jData.dump(2)
						<< "\n";

				calcConstructZTestProportions(iData, jData, i, j, matrixContextMap);
				// calc Z-test for proportions, as a whole and if applicable for each construct when enough samples are present				
				calcGeneralZTestProportions(iData, jData, i, j, matrixContextMap);
				currentChoices.pop_back();
			}
		}
		
		cout << "Start exports \n";
		for(auto [contextKey, contextData] : matrixContextMap){
			cout << outputPath.string() + contextKey + ".csv" << "\n";
			exportZTestsMatrixToCSV(outputPath.string() + contextKey + ".csv", contextData);
		}
	}
	
	void calcConstructZTestProportions(json& iData, json& jData, int i, int j, unordered_map<string, Matrix<double>>& matrixContextMap){
		cout << "calcConstructZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";
		
		auto jConstructs = jData["constructs"];
		for(auto [constructKey, iConstructData] : iData["constructs"].items()){
			cout 	<< constructKey
					<< "\n";

			cout << "TEST0 \n";
			if(jConstructs.contains(constructKey)){
				auto jConstructData = jConstructs[constructKey];

				// calculate success/failure condition
				if(iConstructData.contains("oracleTotal") && jConstructData.contains("oracleTotal")){
					cout << "TEST1 \n";

					auto niOracle = iConstructData["oracleTotal"].get<double>();
					auto piOracle = iConstructData["P(X)"].get<double>();

					auto njOracle = jConstructData["oracleTotal"].get<double>();
					auto pjOracle = jConstructData["P(X)"].get<double>();

					cout << "TEST2 \n";

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
						// use Fisher's Exact Test or other test?
						continue;
					}
					cout << "TEST3 \n";

					// De sample proportions zijn in dit geval hetzelfde als P(X), dus alleen pooled proportions uitrekenen.
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
					cout << "TEST4 \n";

					auto pooledSE = sqrt(
						pooledProportion * (1 - pooledProportion) *
						(
							(1 / niOracle) +
							(1 / njOracle)
						)
					); 
					auto ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
					if(matrixContextMap.find(constructKey) == matrixContextMap.end()){
						cout << ZStat2Proportions << " - creating construct matrix \n";
						matrixContextMap[constructKey] = Matrix<double>(inputDirs.size(),inputDirs.size());
					}
					matrixContextMap[constructKey].at(i,j) = ZStat2Proportions;
					matrixContextMap[constructKey].at(j,i) = ZStat2Proportions;
					cout << "TEST5 \n";

				}
			}
		}
	}
	
	void calcGeneralZTestProportions(json& iData, json& jData, int i, int j, unordered_map<string, Matrix<double>>& matrixContextMap){
		cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

		auto niOracle = iData["metadata"]["oracleTotal"].get<double>();
		auto piOracle = iData["metadata"]["P(X)"].get<double>();

		auto njOracle = jData["metadata"]["oracleTotal"].get<double>();
		auto pjOracle = jData["metadata"]["P(X)"].get<double>();

		cout << "TESTA \n";
		
		auto pooledProportion = 		
		(
			(
				iData["metadata"]["recoveredConstructsTotal"].get<double>() + 
				jData["metadata"]["recoveredConstructsTotal"].get<double>()
			) / (
				niOracle + njOracle
			)
		);
		cout << "TESTB \n";

		auto pooledSE = sqrt(
			pooledProportion * (1-pooledProportion) *
			(
				(1 / niOracle) +
				(1 / njOracle)
			)
		); 
		cout << "TESTC \n";

		auto ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
		
		if(matrixContextMap.find("general") == matrixContextMap.end()){
			cout << ZStat2Proportions << " - creating general matrix \n";
			matrixContextMap["general"] = Matrix<double>(inputDirs.size(),inputDirs.size());
		}
		matrixContextMap["general"].at(i,j) = ZStat2Proportions;
		matrixContextMap["general"].at(j,i) = ZStat2Proportions;
	}
};
	