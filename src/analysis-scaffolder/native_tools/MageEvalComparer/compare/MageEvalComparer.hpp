#include <cmath>
#include <filesystem>
#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

#include "castle.hpp"
#include "paths.hpp"
#include "matrix.hpp"
#include "json.hpp"
#include "binaries.hpp"
#include "stats.hpp"
#include "export.hpp"

using namespace nlohmann;
using namespace std;

/**
 * 	MaveEvalComparer.
 * 	This classes takes the summary.json from each inputdir; then it proceeds to aggregate, recalculate and output the combined summary.
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
		
		unordered_map<string, Matrix<double>> 	constructZMatrixMap, 				// contains constructs - Z value evals related to the summaries.
											 	constructPMatrixMap;				// contains constructs - P value evals related to the summaries.
		Matrix<double>							generalZMatrix,						// contains general    - Z value evals related to the summaries.
												generalPMatrix;						// contains general    - P value evals related to the summaries.

		const string 							summaryFilename = "summary.json",
												binMetadataCmd 	= "rabin2 -j -I ";

		vector<json> 							summaryData, binaryMetadata;
		json									unifiedSummary 	= json::object();
		
		const vector<string>					radare2Keys = {						// Keys to determine which binary metadata to pull from radare2 output.
			"arch",  "bits", "bintype", "class", "compiler", "endian", "lang", "machine", "os"
		};
		
	public:
	MageEvalComparer(vector<filesystem::path> &inputDirs, filesystem::path &outputPath) :
	inputDirs(inputDirs), outputPath(outputPath), 
	
	generalZMatrix(inputDirs.size(), inputDirs.size()),
	generalPMatrix(inputDirs.size(), inputDirs.size())
	{
		sort(inputDirs.begin(),inputDirs.end());
		loadSummaryData();
	}	
		
		void loadSummaryData(){
			for(int i=0 ; i<inputDirs.size() ; i++){
				summaryData.push_back(json::object());
				loadJSON(inputDirs[i].string() + summaryFilename, summaryData[i]);
			}
		}
	
		void gatherBinaryCharacteristics(){
			string dataStringHolder;
			string binaryDirsTarget;
			string command;
			sort(inputDirs.begin(),inputDirs.end());
			for(int i=0 ; i<inputDirs.size() ; i++){
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
	
		void buildZPooledSummaries(){
			for(int i=0 ; i<inputDirs.size() ; i++){
				for(int j = i + 1 ; j<inputDirs.size() ; j++){
					// calc Z-test for proportions, as a whole and if applicable for each construct when enough samples are present	
					calcConstructZTestPooledProportions(summaryData[i], summaryData[j], i, j);
					calcGeneralZTestPooledProportions(summaryData[i], summaryData[j], i, j);
				}
			}
		}
		
		void buildZUnpooledSummaries(){
			for(int i=0 ; i<inputDirs.size() ; i++){
				for(int j = i + 1 ; j<inputDirs.size() ; j++){
					// calc Z-test for proportions, as a whole and if applicable for each construct when enough samples are present	
					calcConstructZTestUnpooledProportions(summaryData[i], summaryData[j], i, j);
					calcGeneralZTestUnpooledProportions(summaryData[i], summaryData[j], i, j);
				}
			}
		}
		
		void startAggregationAndExport(bool &recombine_only){
			createUnifiedSummary();
			saveJsonToFile(outputPath.string() + summaryFilename, unifiedSummary);

			ostringstream oss;
			oss << scientific << setprecision(3);
			unordered_map<string, string> CSVData;

			cout << "Start exports \n";
			buildZPooledSummaries();
			for(auto& [contextKey, contextZData] : constructZMatrixMap){
				cout << outputPath.string() + contextKey + ".csv" << "\n";
									
				if(recombine_only){
					appendConstructMappingOnly(CSVData[contextKey], inputDirs, outputPath, summaryData, contextKey);
				} else {
					appendBinaryCharacteristicsGeneral(CSVData[contextKey], inputDirs, outputPath, summaryData, binaryMetadata, contextKey);
				}	
				appendLabelRow(CSVData[contextKey], "Pooled");
				appendZScores(CSVData[contextKey], inputDirs, contextZData, oss);
				appendPValues(CSVData[contextKey], inputDirs, constructPMatrixMap[contextKey], oss);
			}
			
			if(recombine_only){
				appendGeneralMappingOnly(CSVData["General"], inputDirs, outputPath.string() + "General.csv", summaryData, "General");
			} else {
				appendBinaryCharacteristicsGeneral(CSVData["General"], inputDirs, outputPath.string() + "General.csv", summaryData, binaryMetadata, "General");
			}
			appendLabelRow(CSVData["General"], "Pooled");
			appendZScores(CSVData["General"], inputDirs, generalZMatrix, oss);
			appendPValues(CSVData["General"], inputDirs, generalPMatrix, oss);
			oss.clear();
			
			// Append unpooled text information
			buildZUnpooledSummaries();
			for(auto& [contextKey, contextZData] : constructZMatrixMap){
				cout << outputPath.string() + contextKey + ".csv" << "\n";
				appendLabelRow(CSVData[contextKey], "Unpooled");
				appendZScores(CSVData[contextKey], inputDirs, contextZData, oss);
				appendPValues(CSVData[contextKey], inputDirs, constructPMatrixMap[contextKey], oss);
			}
			appendLabelRow(CSVData["General"], "Unpooled");
			appendZScores(CSVData["General"], inputDirs, generalZMatrix, oss);
			appendPValues(CSVData["General"], inputDirs, generalPMatrix, oss);
			for(auto& [contextKey, contextZData] : constructZMatrixMap){
				string2file(CSVData[contextKey], outputPath.string() + contextKey + ".csv");
			}
			string2file(CSVData["General"], outputPath.string() + "General.csv");
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
								if(n1Val.is_null()){
									n1USCOnstruct[n1Key] = n1USCOnstruct[n1Key].get<int>();
								} else {
									n1USCOnstruct[n1Key] = n1USCOnstruct[n1Key].get<int>() + n1Val.get<int>();
								}
								
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
			
			// recalc new probabilities
			for(auto& [constructKey, constructData] :unifiedSummaryConstructs.items()){
				if(constructData["viableComparisons"].get<double>() > 0 && !constructData["recoveryOfAttr"]["recovered_construct"].is_null()){
					unifiedSummaryConstructs[constructKey]["P(X)"] = constructData["recoveryOfAttr"]["recovered_construct"].get<double>() / constructData["viableComparisons"].get<double>();
					unifiedSummaryConstructs[constructKey]["VAR(X)"] = unifiedSummaryConstructs[constructKey]["P(X)"].get<double>() * ( 1 - unifiedSummaryConstructs[constructKey]["P(X)"].get<double>());
				} else {
					unifiedSummaryConstructs[constructKey]["P(X)"] = 0;
					unifiedSummaryConstructs[constructKey]["VAR(X)"] = 0;
				}
			}

			// guard against some scenario's
			if(unifiedSummaryMetadata["viableComparisons"].get<double>() > 0 && !unifiedSummaryMetadata["recoveredConstructsTotal"].is_null()){
				unifiedSummaryMetadata["P(X)"] = unifiedSummaryMetadata["recoveredConstructsTotal"].get<double>() / unifiedSummaryMetadata["viableComparisons"].get<double>();
				unifiedSummaryMetadata["VAR(X)"] = unifiedSummaryMetadata["P(X)"].get<double>() * (1 - unifiedSummaryMetadata["P(X)"].get<double>());
			} else {
				unifiedSummaryMetadata["P(X)"] = 0;
				unifiedSummaryMetadata["VAR(X)"] = 0;
			}
		}
		
		/**
		 * 	Calculates the Z values for each pair per construct from the inputData and stores these in the ZDataMatrix
		 *	@param	iData	Data of input i from inputDirs.
		 * 	@param	jData	Data of input j from inputDirs.
		 * 	@param	i		The associated row of calculated pair in the matrix
		 * 	@param	j		The associated col of calculated pair in the matrix
		 */
		void calcConstructZTestPooledProportions(json& iData, json& jData, int i, int j){
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

						if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
							continue; // TODO: maybe use Fisher's Exact Test or other test?
						}

						// De sample proportions zijn in dit geval hetzelfde als P(X), dus alleen pooled proportions uitrekenen.
						double pooledProportion = twoProportionsPPooledProportion(
							iConstructData["recoveryOfAttr"]["recovered_construct"].get<double>(), niOracle,
							jConstructData["recoveryOfAttr"]["recovered_construct"].get<double>(), njOracle
						);

						// calc Z statistic, for large disproportions between case 1 and 2 use unpooled (Wald) standard error instead of pooled?
						// We have to make/choose certain assumptions, which validate the approach, i.e. pool vs unpooled. 
						// Do we assume the null hypothesis is true? Or do we remain reserved without assuming H0?
						// 		H0: p_i  = p_j
						//		Ha: p_i != p_j, alternative hypothesis
						// If we assume null is true, then we would be encouraged to use the pooled standard error (SE)
						// If we group the results by the same decompiler, we assume H0, that is the decompiler is the main determinant for found distribution.

						double pooledSE = twoProportionsPooledSE(niOracle, njOracle, pooledProportion);
					
						double ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
						double pValue = erfc(abs(ZStat2Proportions) / sqrt(2.0));
						if(constructZMatrixMap.find(constructKey) == constructZMatrixMap.end()){
//							cout << ZStat2Proportions << " - creating Z construct matrix \n";
							constructZMatrixMap[constructKey] = Matrix<double>(inputDirs.size(),inputDirs.size());
						}
						constructZMatrixMap[constructKey].at(i,j) = ZStat2Proportions;
						constructZMatrixMap[constructKey].at(j,i) = ZStat2Proportions;
						
						if(constructPMatrixMap.find(constructKey) == constructPMatrixMap.end()){
//							cout << PValue << " - creating P construct matrix \n";
							constructPMatrixMap[constructKey] = Matrix<double>(inputDirs.size(),inputDirs.size());
						}
						constructPMatrixMap[constructKey].at(i,j) = pValue;
						constructPMatrixMap[constructKey].at(j,i) = pValue;
					}
				}
			}
		}
		
		/**
		* 	Calculates the Z values for each pair for the overall stats from the inputData and stores these in the ZDataMatrix
		*	@param	iData	Data of input i from inputDirs.
		* 	@param	jData	Data of input j from inputDirs.
		* 	@param	i		The associated row of calculated pair in the matrix
		* 	@param	j		The associated col of calculated pair in the matrix		 
		*/
		void calcGeneralZTestPooledProportions(json& iData, json& jData, int i, int j){
			cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

			auto niOracle = iData["metadata"]["viableComparisons"].get<double>();
			auto piOracle = iData["metadata"]["P(X)"].get<double>();

			auto njOracle = jData["metadata"]["viableComparisons"].get<double>();
			auto pjOracle = jData["metadata"]["P(X)"].get<double>();
			
			if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
				return;
			}
			
			double pooledProportion = twoProportionsPPooledProportion(
				iData["metadata"]["recoveredConstructsTotal"].get<double>(), niOracle, 
				jData["metadata"]["recoveredConstructsTotal"].get<double>(), njOracle
			);

			double pooledSE = twoProportionsPooledSE(niOracle, njOracle, pooledProportion);

			auto ZStat2Proportions = (piOracle - pjOracle) / pooledSE;
			double pValue =  erfc(abs(ZStat2Proportions) / sqrt(2.0));

			generalZMatrix.at(i,j) = ZStat2Proportions;
			generalZMatrix.at(j,i) = ZStat2Proportions;
			generalPMatrix.at(i,j) = pValue;
			generalPMatrix.at(j,i) = pValue;
		}
		
		/**
		 * 	Calculates the Z values for each pair per construct from the inputData and stores these in the ZDataMatrix
		 *	@param	iData	Data of input i from inputDirs.
		 * 	@param	jData	Data of input j from inputDirs.
		 * 	@param	i		The associated row of calculated pair in the matrix
		 * 	@param	j		The associated col of calculated pair in the matrix
		 */
		void calcConstructZTestUnpooledProportions(json& iData, json& jData, int i, int j){
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

						if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
							continue; // TODO: maybe use Fisher's Exact Test or other test?
						}

						// calc Z statistic, for large disproportions between case 1 and 2 use unpooled (Wald) standard error instead of pooled?
						// We have to make/choose certain assumptions, which validate the approach, i.e. pool vs unpooled. 
						// Do we assume the null hypothesis is true? Or do we remain reserved without assuming H0?
						// 		H0: p_i  = p_j
						//		Ha: p_i != p_j, alternative hypothesis
						// If we assume null is true, then we would be encouraged to use the pooled standard error (SE)
						// If we group the results by the same decompiler, we assume H0, that is the decompiler is the main determinant for found distribution.

						double unpooledSE = twoProportionsUnpooledSE(
							iData["metadata"]["VAR(X)"].get<double>(), niOracle, 
							jData["metadata"]["VAR(X)"].get<double>(), njOracle
						);
							
						auto ZStat2Proportions = (piOracle - pjOracle) / unpooledSE;
					
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
		
		/**
		* 	Calculates the Z values for each pair for the overall stats from the inputData and stores these in the ZDataMatrix
		*	@param	iData	Data of input i from inputDirs.
		* 	@param	jData	Data of input j from inputDirs.
		* 	@param	i		The associated row of calculated pair in the matrix
		* 	@param	j		The associated col of calculated pair in the matrix		 
		*/
		void calcGeneralZTestUnpooledProportions(json& iData, json& jData, int i, int j){
			cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

			auto niOracle = iData["metadata"]["viableComparisons"].get<double>();
			auto piOracle = iData["metadata"]["P(X)"].get<double>();

			auto njOracle = jData["metadata"]["viableComparisons"].get<double>();
			auto pjOracle = jData["metadata"]["P(X)"].get<double>();
			
			if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
				return;
			}
			
			double unpooledSE = twoProportionsUnpooledSE(
				iData["metadata"]["VAR(X)"].get<double>(), niOracle, 
				jData["metadata"]["VAR(X)"].get<double>(), njOracle
			);
				
			auto ZStat2Proportions = (piOracle - pjOracle) / unpooledSE;
			
			double pValue =  erfc(abs(ZStat2Proportions) / sqrt(2.0));

			generalZMatrix.at(i,j) = ZStat2Proportions;
			generalZMatrix.at(j,i) = ZStat2Proportions;
			generalPMatrix.at(i,j) = pValue;
			generalPMatrix.at(j,i) = pValue;
		}
};
