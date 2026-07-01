#include <cmath>
#include <filesystem>
#include "castle.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include "../../../../../include/paths.hpp"

#include "matrix.hpp"
#include "json.hpp"
#include "binaries.hpp"

using namespace nlohmann;
using namespace std;

/**
 * 	MaveEvalComparer.
 * 	This classes takes the summary.json from each inputdir, then it proceeds to aggregate , recalculate and output the combined summary.
 *		
 *	TODO: 
 *	Considerations:
 *		- Inclusie oracleMarkedTypes?
 *		DeclStmt is een stmt waar decompilers moeite mee hebben, de lijst toont een glimps van waar mee geworsteld wordt.
 *		Echter is de lijst aanzienlijk en is deze voor de summary misschien overbodig.
 */

class MageEvalComparer{
	private:
		vector<filesystem::path> 				&inputDirs;
		filesystem::path 						&outputPath;						// use outputPath as dir to retrieve previously existing aggregated data from.
		const filesystem::path					buildScaffolderDir = "/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/";
		
		unordered_map<string, Matrix<double>> 	constructZMatrixMap; 				// contains constructs - Z value evals related to the summaries.
		unordered_map<string, Matrix<double>> 	constructPMatrixMap;				// contains constructs - P value evals related to the summaries.
		Matrix<double>							generalZMatrix;						// contains general    - Z value evals related to the summaries.
		Matrix<double>							generalPMatrix;						// contains general    - P value evals related to the summaries.

		const string 							summaryFilename = "summary.json";
		const string 							binMetadataCmd 	= "rabin2 -j -I ";

		vector<json> 							summaryData, binaryMetadata;
		json									unifiedSummary 	= json::object();
		
		// Keys to determine which binary metadata to pull from radare2 output.
		const vector<string>					radare2Keys = {
			"arch",  "bits", "bintype", "class", "compiler", "endian", "lang", "machine", "os"
		};
		
	public:
	MageEvalComparer(vector<filesystem::path> &inputDirs, filesystem::path &outputPath) :
	inputDirs(inputDirs), outputPath(outputPath), 
	
	generalZMatrix(inputDirs.size(), inputDirs.size()),
	generalPMatrix(inputDirs.size(), inputDirs.size())
	{
		string dataStringHolder;
		string binaryDirsTarget;
		string command;
		sort(inputDirs.begin(),inputDirs.end());
		for(int i=0 ; i<inputDirs.size() ; i++){
			summaryData.push_back(json::object());
			loadJSON(inputDirs[i].string() + summaryFilename, summaryData[i]);
			binaryDirsTarget = getCastleFormatBuildNameFromAnalysisName(inputDirs[i].parent_path().filename().string()); 
			for(const auto& entry : filesystem::recursive_directory_iterator(buildScaffolderDir / binaryDirsTarget)){
				// perform extraction and then break, assuming all other related binaries have the same general settings.
				command = binMetadataCmd + (buildScaffolderDir / binaryDirsTarget).string() + "/" + entry.path().filename().string();
//				cout << command << "\n";
				execCommandAndCaptureOutput(command, dataStringHolder);
				break;
			}
			
			binaryMetadata.push_back(json::parse(dataStringHolder)["info"]);
			dataStringHolder.clear();
		}		
	}	
		
		void startComparisons(){
			for(int i=0 ; i<inputDirs.size() ; i++){
				for(int j = i + 1 ; j<inputDirs.size() ; j++){
					// calc Z-test for proportions, as a whole and if applicable for each construct when enough samples are present	
					calcConstructZTestProportions(summaryData[i], summaryData[j], i, j);
					calcGeneralZTestProportions(summaryData[i], summaryData[j], i, j);
				}
			}
			createUnifiedSummary();
			
			cout << "Start exports \n";
			for(auto& [contextKey, contextData] : constructZMatrixMap){
				cout << outputPath.string() + contextKey + ".csv" << "\n";
				exportZConstructMatrixToCSV(outputPath.string() + contextKey + ".csv", contextData, constructPMatrixMap[contextKey], contextKey);
			}
			exportZGeneralMatrixToCSV(outputPath.string() + "General.csv", generalZMatrix, generalPMatrix, "General");
			saveJsonToFile(outputPath.string() + summaryFilename, unifiedSummary);
		}
	
		/**
		 *	Aggregates and recomputes the necessary data related to all the inputdirs.
		 */
		void createUnifiedSummary(){
			cout << "createUnifiedSummary \n";
			// count up basic numbers
			unifiedSummary["constructs"] = json::object();
			unifiedSummary["metadata"] = json::object();
			
			auto& unifiedSummaryConstructs = unifiedSummary["constructs"];
			auto& unifiedSummaryMetadata = unifiedSummary["metadata"];

			for(auto& summary:summaryData){
				// tally up constructs
				for(auto& [constructKey, constructData]:summary["constructs"].items()){
					if(unifiedSummaryConstructs[constructKey].is_null()){
						unifiedSummaryConstructs[constructKey] = json::object();
					}
					auto& n0USConstruct = unifiedSummaryConstructs[constructKey];

					for(auto& [n0Key, n0Val]:constructData.items()){
						if(n0Val.is_object()){ // currently assumes no further nesting

							if(n0USConstruct[n0Key].is_null()){
								n0USConstruct[n0Key] = json::object();
							}
							auto& n1USCOnstruct = n0USConstruct[n0Key];

							for(auto& [n1Key, n1Val]: n0Val.items()){
								if(n1USCOnstruct[n1Key].is_null()){
									n1USCOnstruct[n1Key] = 0;
								}
								n1USCOnstruct[n1Key] = n1USCOnstruct[n1Key].get<int>() + n1Val.get<int>();
							}
							
						} else {
							if(n0USConstruct[n0Key].is_null()){
								n0USConstruct[n0Key] = 0;
							}
							n0USConstruct[n0Key] = n0USConstruct[n0Key].get<int>() + n0Val.get<int>();
						}
					}

								}
				// Tally up metadata
				for(auto& [mKey, mData]:summary["metadata"].items()){
					if(unifiedSummaryMetadata[mKey].is_null()){
						unifiedSummaryMetadata[mKey] = 0;
					}
					unifiedSummaryMetadata[mKey] = unifiedSummaryMetadata[mKey].get<int>() + mData.get<int>();
				}

			}
			
			for(auto& [constructKey, constructData] :unifiedSummaryConstructs.items()){
				if(constructData["viableComparisons"].get<double>() > 0 && !constructData["recoveryOfAttr"]["recovered_construct"].is_null()){
					unifiedSummaryConstructs[constructKey]["P(X)"] = constructData["recoveryOfAttr"]["recovered_construct"].get<double>() / constructData["viableComparisons"].get<double>();
					unifiedSummaryConstructs[constructKey]["VAR(X)"] = unifiedSummaryConstructs[constructKey]["P(X)"].get<double>() * ( 1 - unifiedSummaryConstructs[constructKey]["P(X)"].get<double>());
				} else {
					unifiedSummaryConstructs[constructKey]["P(X)"] = 0;
					unifiedSummaryConstructs[constructKey]["VAR(X)"] = 0;
				}
			}

			if(unifiedSummaryMetadata["viableComparisons"].get<double>() > 0 && !unifiedSummaryMetadata["recoveredConstructsTotal"].is_null()){
				unifiedSummaryMetadata["P(X)"] = unifiedSummaryMetadata["recoveredConstructsTotal"].get<double>() / unifiedSummaryMetadata["viableComparisons"].get<double>();
				unifiedSummaryMetadata["VAR(X)"] = unifiedSummaryMetadata["P(X)"].get<double>() * (1 - unifiedSummaryMetadata["P(X)"].get<double>());
			} else {
				unifiedSummaryMetadata["P(X)"] = 0;
				unifiedSummaryMetadata["VAR(X)"] = 0;
			}
		}
		
		template <typename T>
		void exportZConstructMatrixToCSV(const string& path, Matrix<T>& contextZData, Matrix<T>& contextPData, const string& contextKey){
			ostringstream oss;
			oss << scientific << setprecision(3);
			
			// Display general characteristics
			string CSVData = "EntryMapping," + contextKey + ",viableComparisons";
			
			for(auto key : radare2Keys){
				CSVData += "," + key;
			}
			CSVData += "\n";
			
			for(int i = 0; i<inputDirs.size();i++){
				int vcCount = 0;
				if(summaryData[i]["constructs"].contains(contextKey)){
					vcCount = summaryData[i]["constructs"][contextKey]["viableComparisons"].get<int>();
				}
				CSVData += 
					to_string(i) 																			+ "," + 
					get_leaf_string(inputDirs[i].string()) 												+ "," +
					to_string(vcCount);
				
				for(auto key : radare2Keys){
					if(binaryMetadata[i][key].is_string()){
						CSVData	+= "," + binaryMetadata[i][key].get<string>();	
					} else if(binaryMetadata[i][key].is_number_integer()){
						CSVData	+= "," + to_string(binaryMetadata[i][key].get<int>());
					} else {
						CSVData	+= ", ";
					}
				}	
				CSVData += "\n";	
			}
			
			
			CSVData += "\n";
			
			// Z-Score
			CSVData += "ZScore";
			for(int i = 0; i<inputDirs.size();i++){
				CSVData += "," + to_string(i) ;
			}
			CSVData += "\n";

			for(int i=0 ; i<inputDirs.size() ; i++){
				CSVData += to_string(i);
				for(int j = 0 ; j<inputDirs.size() ; j++){
					if(i!=j && contextZData.at(i, j) == 0){
						CSVData += ",-";
					} else {
						oss << contextZData.at(i, j);
						CSVData += "," + oss.str();
						oss.clear();
						oss.str("");
					}
				}
				CSVData += "\n";
			}
			
			// P-Value
			CSVData += "\nPValue";
			for(int i = 0; i<inputDirs.size();i++){
				CSVData += "," + to_string(i) ;
			}
			CSVData += "\n";

			for(int i=0 ; i<inputDirs.size() ; i++){
				CSVData += to_string(i);
				for(int j = 0 ; j<inputDirs.size() ; j++){
					if(i!=j && contextPData.at(i, j) == 0){
						CSVData += ",-";
					} else {
						oss << contextPData.at(i, j);
						CSVData += "," + oss.str();
						oss.clear();
						oss.str("");
					}
				}
				CSVData += "\n";
			}
			ofstream file(path);
		    if (!file.is_open()) {
		        std::cerr << "Error: Could not open file for writing: " << path << "\n";
		        return;
		    }
		    file << CSVData;
		}
	
		/**
		 *	Exports the data for the general criteria to csv format.
		 */
		template <typename T>
		void exportZGeneralMatrixToCSV(const string& path, Matrix<T>& contextZData, Matrix<T>& contextPData, const string& contextKey){
			ostringstream oss;
			oss << scientific << setprecision(3);
			// export construct and general values to CSV
			string CSVData = "EntryMapping," + contextKey + ",viableComparisons";
			
			for(auto key : radare2Keys){
				CSVData += "," + key;
			}
			CSVData += "\n";
			
			for(int i = 0; i<inputDirs.size();i++){
				int vcCount = summaryData[i]["metadata"]["viableComparisons"].get<int>();
				CSVData += 
					to_string(i) 								+ "," + 
					get_leaf_string(inputDirs[i].string()) 	+ "," +
					to_string(vcCount);
					
					for(auto key : radare2Keys){
					if(binaryMetadata[i][key].is_string()){
						CSVData	+= "," + binaryMetadata[i][key].get<string>();	
					} else if(binaryMetadata[i][key].is_number_integer()){
						CSVData	+= "," + to_string(binaryMetadata[i][key].get<int>());
					} else {
						CSVData	+= ", ";
					}
				}	
				CSVData += "\n";	
			}
			CSVData += "\n";
			
			CSVData += "ZScore";
			for(int i = 0; i<inputDirs.size();i++){
				CSVData += "," + to_string(i) ;
			}
			CSVData += "\n";

			for(int i=0 ; i<inputDirs.size() ; i++){
				CSVData += to_string(i);
				for(int j = 0 ; j<inputDirs.size() ; j++){
					if(i!=j && contextZData.at(i, j) == 0){
						CSVData += ",-";
					} else {
						oss << contextZData.at(i, j);
						CSVData += "," + oss.str();
						oss.clear();
						oss.str("");
					}
				}
				CSVData += "\n";
			}
			
			CSVData += "\nPValue";
			for(int i = 0; i<inputDirs.size();i++){
				CSVData += "," + to_string(i) ;
			}
			CSVData += "\n";

			for(int i=0 ; i<inputDirs.size() ; i++){
				CSVData += to_string(i);
				for(int j = 0 ; j<inputDirs.size() ; j++){
					if(i!=j && contextPData.at(i, j) == 0){
						CSVData += ",-";
					} else {
						oss << contextPData.at(i, j);
						CSVData += "," + oss.str();
						oss.clear();
						oss.str("");
					}
				}
				CSVData += "\n";
			}
			ofstream file(path);
		    if (!file.is_open()) {
		        std::cerr << "Error: Could not open file for writing: " << path << "\n";
		        return;
		    }
		    file << CSVData;
		}
		
		/**
		 *	
		 */
		void calcConstructZTestProportions(json& iData, json& jData, int i, int j){
			cout << "calcConstructZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";
			
			auto jConstructs = jData["constructs"];
			for(auto [constructKey, iConstructData] : iData["constructs"].items()){

				if(jConstructs.contains(constructKey)){
					auto jConstructData = jConstructs[constructKey];

					// calculate success/failure condition
					if(iConstructData.contains("viableComparisons") && jConstructData.contains("viableComparisons")){
						double niOracle = iConstructData["viableComparisons"].get<double>();
						double piOracle = iConstructData["P(X)"].get<double>();

						double njOracle = jConstructData["viableComparisons"].get<double>();
						double pjOracle = jConstructData["P(X)"].get<double>();

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
							// TODO: use Fisher's Exact Test or other test?
							continue;
						}

						// De sample proportions zijn in dit geval hetzelfde als P(X), dus alleen pooled proportions uitrekenen.

						double pooledProportion = (
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

						double pooledSE = sqrt(
							pooledProportion * (1 - pooledProportion) *
							(
								(1 / niOracle) +
								(1 / njOracle)
							)
						); 
						double ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
						double pValue = erfc(abs(ZStat2Proportions) / sqrt(2.0));
						if(constructZMatrixMap.find(constructKey) == constructZMatrixMap.end()){
//							cout << ZStat2Proportions << " - creating construct matrix \n";
							constructZMatrixMap[constructKey] = Matrix<double>(inputDirs.size(),inputDirs.size());
						}
						constructZMatrixMap[constructKey].at(i,j) = ZStat2Proportions;
						constructZMatrixMap[constructKey].at(j,i) = ZStat2Proportions;
						
						if(constructPMatrixMap.find(constructKey) == constructPMatrixMap.end()){
//							cout << PValue << " - creating construct matrix \n";
							constructPMatrixMap[constructKey] = Matrix<double>(inputDirs.size(),inputDirs.size());
						}
						constructPMatrixMap[constructKey].at(i,j) = pValue;
						constructPMatrixMap[constructKey].at(j,i) = pValue;
					}
				}
			}
		}
		
		void calcGeneralZTestProportions(json& iData, json& jData, int i, int j){
			cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

			auto niOracle = iData["metadata"]["viableComparisons"].get<double>();
			auto piOracle = iData["metadata"]["P(X)"].get<double>();

			auto njOracle = jData["metadata"]["viableComparisons"].get<double>();
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
			double pValue =  erfc(abs(ZStat2Proportions) / sqrt(2.0));

			generalZMatrix.at(i,j) = ZStat2Proportions;
			generalZMatrix.at(j,i) = ZStat2Proportions;
			generalPMatrix.at(i,j) = pValue;
			generalPMatrix.at(j,i) = pValue;
		}
};
