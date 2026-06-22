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
#include "matrix.hpp"
#include "json.hpp"

using namespace nlohmann;
using namespace std;

// TODO: SE 
class MageEvalComparer{
	private:
		vector<filesystem::path> &inputDirs;
		filesystem::path &outputPath;							// use outputPath as dir to retrieve previously existing aggregated data from.
		
		unordered_map<string, Matrix<double>> matrixContextMap; // contains contexts general and the constructs from the summaries.
		const string summaryFilename = "summary.json";

	public:
	MageEvalComparer(vector<filesystem::path> &inputDirs, filesystem::path &outputPath) :
	inputDirs(inputDirs), outputPath(outputPath)
	{
		
	}	
	
		void startComparisons(){
			sort(inputDirs.begin(),inputDirs.end());
			vector<filesystem::path> currentChoices;
			json iData, jData;
			
			for(int i=0 ; i<inputDirs.size() ; i++){
				loadJSON(inputDirs[i].string() + summaryFilename, iData);
				currentChoices.push_back(inputDirs[i]);
	//			cout 	<< inputDirs[i].string() + summaryFilename << "\n"
	//					<< iData.dump(2) 
	//					<< "\n";

				for(int j = i + 1 ; j<inputDirs.size() ; j++){
					loadJSON(inputDirs[j].string() + summaryFilename, jData);
					currentChoices.push_back(inputDirs[j]);
	//				cout 	<< inputDirs[j].string() + summaryFilename << "\n"
	//						<< jData.dump(2)
	//						<< "\n";

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
	
	// TODO: if no viable comparison is possible, output blank instead of 0 for i!=j
		template <typename T>
		void exportZTestsMatrixToCSV(const string& path, Matrix<T>& contextData){
			// export construct and general values to CSV
			string CSVData 
				= "EntryMapping \n"
				;
			for(int i = 0; i<inputDirs.size();i++){
				CSVData = CSVData + to_string(i) + "," + get_leaf_string(inputDirs[i].string()) + "\n";
			}
			CSVData = CSVData + "\n";
			
			CSVData = CSVData + "ZScore";
			for(int i = 0; i<inputDirs.size();i++){
				CSVData = CSVData + "," + to_string(i) ;
			}
			CSVData = CSVData + "\n";

			for(int i=0 ; i<inputDirs.size() ; i++){
				CSVData = CSVData + to_string(i);
				for(int j = 0 ; j<inputDirs.size() ; j++){
					if(i!=j && contextData.at(i, j) == 0){
						CSVData = CSVData + ",-";
					} else {
						CSVData = CSVData + "," + to_string(contextData.at(i, j));
					}
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
	
		void calcConstructZTestProportions(json& iData, json& jData, int i, int j, unordered_map<string, Matrix<double>>& matrixContextMap){
			cout << "calcConstructZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";
			
			auto jConstructs = jData["constructs"];
			for(auto [constructKey, iConstructData] : iData["constructs"].items()){
	//			cout 	<< constructKey
	//					<< "\n";

				if(jConstructs.contains(constructKey)){
					auto jConstructData = jConstructs[constructKey];

					// calculate success/failure condition
					if(iConstructData.contains("oracleTotal") && jConstructData.contains("oracleTotal")){

						auto niOracle = iConstructData["oracleTotal"].get<double>();
						auto piOracle = iConstructData["P(X)"].get<double>();

						auto njOracle = jConstructData["oracleTotal"].get<double>();
						auto pjOracle = jConstructData["P(X)"].get<double>();

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
						// We have to make/choose certain assumptions, which validate the approach, i.e. pool vs unpooled. 
						// Do we assume the null hypothesis is true? Or do we remain reserved without assuming H0?
						// 		H0: p_i  = p_j
						//		Ha: p_i != p_j, alternative hypothesis
						// If we assume null is true, then we would be encouraged to use the pooled standard error (SE)
						// If we group the results by the same decompiler, we assume H0, that is the decompiler is the main determinant for found distribution.

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

					}
				}
			}
		}
		
		void calcGeneralZTestProportions(json& iData, json& jData, int i, int j, unordered_map<string, Matrix<double>>& matrixContextMap){
			cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

			auto niOracle = iData["metadata"]["viableComparisonsTotal"].get<double>();
			auto piOracle = iData["metadata"]["P(X)"].get<double>();

			auto njOracle = jData["metadata"]["viableComparisonsTotal"].get<double>();
			auto pjOracle = jData["metadata"]["P(X)"].get<double>();
			
			auto pooledProportion = 		
			(
				(
					iData["metadata"]["recoveredConstructsTotal"].get<double>() + 
					jData["metadata"]["recoveredConstructsTotal"].get<double>()
				) / (
					niOracle + njOracle
				)
			);

			auto pooledSE = sqrt(
				pooledProportion * (1-pooledProportion) *
				(
					(1 / niOracle) +
					(1 / njOracle)
				)
			); 

			auto ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
			
			if(matrixContextMap.find("general") == matrixContextMap.end()){
				cout << ZStat2Proportions << " - creating general matrix \n";
				matrixContextMap["general"] = Matrix<double>(inputDirs.size(),inputDirs.size());
			}
			matrixContextMap["general"].at(i,j) = ZStat2Proportions;
			matrixContextMap["general"].at(j,i) = ZStat2Proportions;
		}
	
};
