#include <cmath>
#include <filesystem>
#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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
 * 	MageEvalComparer.
 * 	This classes takes the summary.json from each inputdir; then it proceeds to aggregate, recalculate and output the combined summary.
 *		
 */

class MageEvalComparer{
	private:
		vector<filesystem::path> 				&inputDirs;
		filesystem::path 						&outputPath;						// use outputPath as dir to retrieve previously existing aggregated data from.
		filesystem::path						&buildScaffolderDir;
		
		unordered_map<string, Matrix<long double>> 	constructZMatrixMap, 			// contains constructs - Z value evals related to the summaries.
											 		constructPMatrixMap;			// contains constructs - P value evals related to the summaries.
		unordered_map<string, Matrix<bool>>     constructInclusionMatrixMap;										
												
		unordered_map<string, Matrix<string>>	constructCIMatrixMap;				// contains constructs - confidence intervals related to the summaries.
		
		Matrix<long double>						generalZMatrix,						// contains general    - Z value evals related to the summaries.
												generalPMatrix;						// contains general    - P value evals related to the summaries.
		Matrix<string>							generalCIMatrix;					// contains general	   - confidence intervals related to the summaries.
		Matrix<bool>							generalInclusionMatrix;
		const string 							summaryFilename = "summary.json",
												radareCmd 	= "rabin2 -j -I ",
												dieCmd 		= "diec -a --json ";
		vector<json> 							summaryData, binaryMetadata;
		json									unifiedSummary = json::object();
		
	public:
	MageEvalComparer(vector<filesystem::path> &inputDirs, filesystem::path &outputPath, filesystem::path &buildScaffolderDir) :
	inputDirs(inputDirs), outputPath(outputPath), buildScaffolderDir(buildScaffolderDir),
	
	generalZMatrix(inputDirs.size(), inputDirs.size()),
	generalPMatrix(inputDirs.size(), inputDirs.size()),
	generalCIMatrix(inputDirs.size(), inputDirs.size()),
	generalInclusionMatrix(inputDirs.size(), inputDirs.size())
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
	
		/**
		 *	Extracts characteristics of the associated binary of some codebase
		 */
		void gatherBinaryCharacteristics(){
			string radareDataStringHolder, diecDataStringHolder;
			string binaryDirsTarget;
			string command;
			for(int i=0 ; i<inputDirs.size() ; i++){
				binaryDirsTarget = getCastleFormatBuildNameFromAnalysisName(inputDirs[i].parent_path().filename().string()); 
				for(const auto& entry : filesystem::recursive_directory_iterator(buildScaffolderDir / binaryDirsTarget)){
					// perform extraction and then break, assuming all other related binaries have the same general settings.
					auto fileBuildPath = (buildScaffolderDir / binaryDirsTarget).string() + "/" + entry.path().filename().string();
					command = radareCmd + fileBuildPath;
					execCommandAndCaptureOutput(command, radareDataStringHolder);
					command = dieCmd + fileBuildPath;
					execCommandAndCaptureOutput(command, diecDataStringHolder);
					break;
				}
				json radareJSON = json::parse(radareDataStringHolder)["info"];
				diecDataStringHolder = extractDiecJsonOnly(diecDataStringHolder);
				json dieJSON = json::parse(diecDataStringHolder);

				if(radareJSON["compiler"].get<string>().empty()){
					for(auto [bIndex, bFinding]:dieJSON["detects"].items()){
	//					cout << bFinding.dump(2) << "\n";
						for(auto [vIndex, vFinding]:bFinding["values"].items()){
							if(vFinding["type"].get<string>() == "compiler"){
								radareJSON["compiler"] = 
									radareJSON["compiler"].get<string>() 	+ 
									vFinding["name"].get<string>() 			+ 
									vFinding["version"].get<string>() 		+ 
									" ";
							}
						}
					}
				}
				
				binaryMetadata.push_back(radareJSON);				
				radareDataStringHolder.clear();
				diecDataStringHolder.clear();
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
		
		void buildHolmBonferroniScores(
			Matrix<long double> &PMatrix, 
			vector<pair<string, long double>> &hbPVector, 
			vector<long double> &hbValueVector, 
			Matrix<bool> &inclMatrix
		){
			long double alphaConfidence = 0.05;
			for(int i=0;i<inputDirs.size();i++){
				for(int j=i+1;j<inputDirs.size();j++){
					if(inclMatrix.get(i, j)){
						hbPVector.push_back({to_string(i) + "-" + to_string(j), PMatrix.at(i, j)});
					}
				}
			}
			sort(hbPVector.begin(), hbPVector.end(),[](const auto& a, const auto& b) {return a.second < b.second;});
			for(int i=0;i<hbPVector.size();i++){
				// calc holm threshold
				hbValueVector.push_back((alphaConfidence / (hbPVector.size() - i)));
			}
		}
		
		void startAggregationAndExport(bool &recombine_only){
			cout << "startAggregationAndExport \n";
			createUnifiedSummary();
			saveJsonToFile(outputPath.string() + summaryFilename, unifiedSummary);
			ostringstream oss;
			oss << scientific << setprecision(3);
			unordered_map<string, string> CSVData;
			vector<pair<string, long double>> hbPVector;
			vector<long double> hbValueVector;
			
			buildZPooledSummaries();
			// Append pooled text information
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
				buildHolmBonferroniScores(constructPMatrixMap[contextKey], hbPVector, hbValueVector, constructInclusionMatrixMap[contextKey]);
				appendHolmBonferroniScores(CSVData[contextKey], inputDirs, hbPVector, hbValueVector, constructCIMatrixMap[contextKey], oss);
				hbValueVector.clear();
				hbPVector.clear();
			}
			
			if(recombine_only){
				appendGeneralMappingOnly(CSVData["General"], inputDirs, outputPath.string() + "General.csv", summaryData, "General");
			} else {
				appendBinaryCharacteristicsGeneral(CSVData["General"], inputDirs, outputPath.string() + "General.csv", summaryData, binaryMetadata, "General");
			}
			appendLabelRow(CSVData["General"], "Pooled");
			appendZScores(CSVData["General"], inputDirs, generalZMatrix, oss);
			appendPValues(CSVData["General"], inputDirs, generalPMatrix, oss);
			buildHolmBonferroniScores(generalPMatrix, hbPVector, hbValueVector, generalInclusionMatrix);
			appendHolmBonferroniScores(CSVData["General"], inputDirs, hbPVector, hbValueVector,generalCIMatrix, oss);
			hbValueVector.clear();
			hbPVector.clear();
			oss.clear();
			
			buildZUnpooledSummaries();
			// Append unpooled text information
			for(auto& [contextKey, contextZData] : constructZMatrixMap){
				cout << outputPath.string() + contextKey + ".csv" << "\n";
				appendLabelRow(CSVData[contextKey], "Unpooled");
				appendZScores(CSVData[contextKey], inputDirs, contextZData, oss);
				appendPValues(CSVData[contextKey], inputDirs, constructPMatrixMap[contextKey], oss);
				buildHolmBonferroniScores(constructPMatrixMap[contextKey], hbPVector, hbValueVector, constructInclusionMatrixMap[contextKey]);
				appendHolmBonferroniScores(CSVData[contextKey], inputDirs, hbPVector, hbValueVector, constructCIMatrixMap[contextKey], oss);
				hbValueVector.clear();
				hbPVector.clear();
			}
			
			appendLabelRow(CSVData["General"], "Unpooled");
			appendZScores(CSVData["General"], inputDirs, generalZMatrix, oss);
			appendPValues(CSVData["General"], inputDirs, generalPMatrix, oss);
			buildHolmBonferroniScores(generalPMatrix, hbPVector, hbValueVector, generalInclusionMatrix);
			appendHolmBonferroniScores(CSVData["General"], inputDirs, hbPVector, hbValueVector,generalCIMatrix, oss);
			
			// Save info to files
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

			unifiedSummary["constructs"] = json::object();
			unifiedSummary["metadata"] = json::object();
			
			auto& unifiedSummaryConstructs = unifiedSummary["constructs"];
			auto& unifiedSummaryMetadata = unifiedSummary["metadata"];

			for(json& summary: summaryData){

				// tally up constructs
				for(auto& [constructKey, constructData]:summary["constructs"].items()){
					if(unifiedSummaryConstructs[constructKey].is_null()){
						unifiedSummaryConstructs[constructKey] = json::object();
					}
					auto& USConstruct = unifiedSummaryConstructs[constructKey];
					for(auto& [summaryKey, summaryAttr]:constructData.items()){
						if(summaryAttr.is_object()){ // currently assumes no further nesting
							if(USConstruct[summaryKey].is_null()){
								USConstruct[summaryKey] = json::object();
							}
							auto& USConstructAttrObj = USConstruct[summaryKey];

							for(auto& [attrKey, attrVal]: summaryAttr.items()){
								if(attrVal.is_number_integer()){
									if(USConstructAttrObj[attrKey].is_null()){
										USConstructAttrObj[attrKey] = 0;
									}
									USConstructAttrObj[attrKey] = USConstructAttrObj[attrKey].get<int>() + attrVal.get<int>();
								}
							}
						} else if(summaryAttr.is_number_integer()){
							if(USConstruct[summaryKey].is_null()){
								USConstruct[summaryKey] = 0;
							}
							USConstruct[summaryKey] = USConstruct[summaryKey].get<int>() + summaryAttr.get<int>();
						} else {
							// P(X) and VAR belong here
						}
					}
				}
				// Tally up metadata
				for(auto& [mKey, mData]:summary["metadata"].items()){
					if(unifiedSummaryMetadata[mKey].is_null()){
						unifiedSummaryMetadata[mKey] = 0;
					}
					if(unifiedSummaryMetadata[mKey].is_number_integer()){
						unifiedSummaryMetadata[mKey] = unifiedSummaryMetadata[mKey].get<int>() + mData.get<int>();
					}
				}
			}
			
			// recalc new probabilities
			for(auto& [constructKey, constructData] :unifiedSummaryConstructs.items()){
				if(constructData["viableComparisons"].get<long double>() > 0 && !constructData["recoveryOfAttr"]["recovered_construct"].is_null()){
					unifiedSummaryConstructs[constructKey]["P(X)"] = constructData["recoveryOfAttr"]["recovered_construct"].get<long double>() / constructData["viableComparisons"].get<long double>();
					unifiedSummaryConstructs[constructKey]["VAR(X)"] = unifiedSummaryConstructs[constructKey]["P(X)"].get<long double>() * ( 1 - unifiedSummaryConstructs[constructKey]["P(X)"].get<long double>());
				} else {
					unifiedSummaryConstructs[constructKey]["P(X)"] = 0;
					unifiedSummaryConstructs[constructKey]["VAR(X)"] = 0;
				}
			}

			// guard against some scenario's
			if(unifiedSummaryMetadata["viableComparisons"].get<long double>() > 0 && !unifiedSummaryMetadata["recoveredConstructsTotal"].is_null()){
				unifiedSummaryMetadata["P(X)"] = unifiedSummaryMetadata["recoveredConstructsTotal"].get<long double>() / unifiedSummaryMetadata["viableComparisons"].get<long double>();
				unifiedSummaryMetadata["VAR(X)"] = unifiedSummaryMetadata["P(X)"].get<long double>() * (1 - unifiedSummaryMetadata["P(X)"].get<long double>());
			} else {
				unifiedSummaryMetadata["P(X)"] = 0;
				unifiedSummaryMetadata["VAR(X)"] = 0;
			}
		}
		
		/**
		 * 	Calculates the Z values and two-tailed P-values for each pair per construct from the inputData and stores these in the ZDataMatrix
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
						long double niOracle = iConstructData["viableComparisons"].get<long double>();
						long double piOracle = iConstructData["P(X)"].get<long double>();

						long double njOracle = jConstructData["viableComparisons"].get<long double>();
						long double pjOracle = jConstructData["P(X)"].get<long double>();

						if(constructInclusionMatrixMap.find(constructKey) == constructInclusionMatrixMap.end()){
							constructInclusionMatrixMap[constructKey] = Matrix<bool>(inputDirs.size(),inputDirs.size());
						}
						if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
							constructInclusionMatrixMap[constructKey].set(i,j, false);
							continue; // TODO: maybe use Fisher's Exact Test or other test? Samples invalid so we skip. P keep initial 0's.
						} else {
							constructInclusionMatrixMap[constructKey].set(i,j, true);
						}

						// De sample proportions zijn in dit geval hetzelfde als P(X), dus alleen pooled proportions uitrekenen.
						long double pooledProportion = twoProportionsPPooledProportion(
							iConstructData["recoveryOfAttr"]["recovered_construct"].get<long double>(), niOracle,
							jConstructData["recoveryOfAttr"]["recovered_construct"].get<long double>(), njOracle
						);

						// calc Z statistic, for large disproportions between case 1 and 2 use unpooled (Wald) standard error instead of pooled?
						// We have to make/choose certain assumptions, which validate the approach, i.e. pool vs unpooled. 
						// Do we assume the null hypothesis is true? Or do we remain reserved without assuming H0?
						// 		H0: p_i  = p_j
						//		Ha: p_i != p_j, alternative hypothesis
						// If we assume null is true, then we would be encouraged to use the pooled standard error (SE)
						// If we group the results by the same decompiler, we assume H0, that is the decompiler is the main determinant for found distribution.

						long double pooledSE = twoProportionsPooledSE(niOracle, njOracle, pooledProportion);
						long double zStat = (piOracle - pjOracle) / pooledSE;
						long double pValue = erfc(abs(zStat) / sqrt(2.0));
						if(constructZMatrixMap.find(constructKey) == constructZMatrixMap.end()){
//							cout << ZStat2Proportions << " - creating Z construct matrix \n";
							constructZMatrixMap[constructKey] = Matrix<long double>(inputDirs.size(),inputDirs.size());
						}
						constructZMatrixMap[constructKey].at(i,j) = zStat;
						constructZMatrixMap[constructKey].at(j,i) = zStat;
						
						if(constructPMatrixMap.find(constructKey) == constructPMatrixMap.end()){
//							cout << PValue << " - creating P construct matrix \n";
							constructPMatrixMap[constructKey] = Matrix<long double>(inputDirs.size(),inputDirs.size());
						}
						constructPMatrixMap[constructKey].at(i,j) = pValue;
						constructPMatrixMap[constructKey].at(j,i) = pValue;
						
						if(constructCIMatrixMap.find(constructKey) == constructCIMatrixMap.end()){
							constructCIMatrixMap[constructKey] = Matrix<string>(inputDirs.size(),inputDirs.size());
						}
						calcConfidenceInterval(piOracle, pjOracle, i, j, pooledSE, constructCIMatrixMap[constructKey]);
					}
				}
			}
		}
		
		/**
		* 	Calculates the Z values and two-tailed P-values for each pair for the overall stats from the inputData and stores these in the ZDataMatrix
		*	@param	iData	Data of input i from inputDirs.
		* 	@param	jData	Data of input j from inputDirs.
		* 	@param	i		The associated row of calculated pair in the matrix
		* 	@param	j		The associated col of calculated pair in the matrix		 
		*/
		void calcGeneralZTestPooledProportions(json& iData, json& jData, int i, int j){
			cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

			long double niOracle = iData["metadata"]["viableComparisons"].get<long double>();
			long double piOracle = iData["metadata"]["P(X)"].get<long double>();

			long double njOracle = jData["metadata"]["viableComparisons"].get<long double>();
			long double pjOracle = jData["metadata"]["P(X)"].get<long double>();
			
			if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
				generalInclusionMatrix.set(i,j, false);
				return;
			} else {
				generalInclusionMatrix.set(i,j, true);
			}
			
			long double pooledProportion = twoProportionsPPooledProportion(
				iData["metadata"]["recoveredConstructsTotal"].get<long double>(), niOracle, 
				jData["metadata"]["recoveredConstructsTotal"].get<long double>(), njOracle
			);

			long double pooledSE = twoProportionsPooledSE(niOracle, njOracle, pooledProportion);
			long double zStat = (piOracle - pjOracle) / pooledSE;
			long double pValue =  erfc(abs(zStat) / sqrt(2.0));

			generalZMatrix.at(i,j) = zStat;
			generalZMatrix.at(j,i) = zStat;
			generalPMatrix.at(i,j) = pValue;
			generalPMatrix.at(j,i) = pValue;
			calcConfidenceInterval(piOracle, pjOracle, i, j, pooledSE, generalCIMatrix);

		}
		
		/**
		 * 	Calculates the Z values and two-tailed P-values for each pair per construct from the inputData and stores these in the ZDataMatrix
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
					if(iConstructData.contains("viableComparisons") && jConstructData.contains("viableComparisons")){
						long double niOracle = iConstructData["viableComparisons"].get<long double>();
						long double piOracle = iConstructData["P(X)"].get<long double>();

						long double njOracle = jConstructData["viableComparisons"].get<long double>();
						long double pjOracle = jConstructData["P(X)"].get<long double>();

						// success/failure condition expectation test for 3 sds, allows normal approximation to a binomial distribution
						// ensure both are eligible for proportion test

						if(constructInclusionMatrixMap.find(constructKey) == constructInclusionMatrixMap.end()){
							constructInclusionMatrixMap[constructKey] = Matrix<bool>(inputDirs.size(),inputDirs.size());
						}
						if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
							constructInclusionMatrixMap[constructKey].set(i,j, false);
							continue; // TODO: maybe use Fisher's Exact Test or other test?
						} else {
							constructInclusionMatrixMap[constructKey].set(i,j, true);
						}

						// calc Z statistic, for large disproportions between case 1 and 2 use unpooled (Wald) standard error instead of pooled?
						// We have to make/choose certain assumptions, which validate the approach, i.e. pool vs unpooled. 
						// Do we assume the null hypothesis is true? Or do we remain reserved without assuming H0?
						// 		H0: p_i  = p_j
						//		Ha: p_i != p_j, alternative hypothesis
						// If we assume null is true, then we would be encouraged to use the pooled standard error (SE)
						// If we group the results by the same decompiler, we assume H0, that is the decompiler is the main determinant for found distribution.

						long double unpooledSE = twoProportionsUnpooledSE(
							iData["metadata"]["VAR(X)"].get<long double>(), niOracle, 
							jData["metadata"]["VAR(X)"].get<long double>(), njOracle
						);
							
						long double zStat = (piOracle - pjOracle) / unpooledSE;
					
						long double pValue = erfc(abs(zStat) / sqrt(2.0));
						if(constructZMatrixMap.find(constructKey) == constructZMatrixMap.end()){
//							cout << ZStat2Proportions << " - creating construct matrix \n";
							constructZMatrixMap[constructKey] = Matrix<long double>(inputDirs.size(),inputDirs.size());
						}
						constructZMatrixMap[constructKey].at(i,j) = zStat;
						constructZMatrixMap[constructKey].at(j,i) = zStat;
						
						if(constructPMatrixMap.find(constructKey) == constructPMatrixMap.end()){
//							cout << PValue << " - creating construct matrix \n";
							constructPMatrixMap[constructKey] = Matrix<long double>(inputDirs.size(),inputDirs.size());
						}
						constructPMatrixMap[constructKey].at(i,j) = pValue;
						constructPMatrixMap[constructKey].at(j,i) = pValue;
						
						if(constructCIMatrixMap.find(constructKey) == constructCIMatrixMap.end()){
							constructCIMatrixMap[constructKey] = Matrix<string>(inputDirs.size(),inputDirs.size());
						}
						calcConfidenceInterval(piOracle, pjOracle, i, j, unpooledSE, constructCIMatrixMap[constructKey]);
					}
				}
			}
		}
		
		/**
		* 	Calculates the unpooled Z values and two-tailed P-values for each pair for the overall stats from the inputData and stores these in the ZDataMatrix
		*	@param	iData	Data of input i from inputDirs.
		* 	@param	jData	Data of input j from inputDirs.
		* 	@param	i		The associated row of calculated pair in the matrix
		* 	@param	j		The associated col of calculated pair in the matrix		 
		*/
		void calcGeneralZTestUnpooledProportions(json& iData, json& jData, int i, int j){
			cout << "calcGeneralZTestProportions " << to_string(i) << "-" << to_string(j) << "\n";

			long double niOracle = iData["metadata"]["viableComparisons"].get<long double>();
			long double piOracle = iData["metadata"]["P(X)"].get<long double>();

			long double njOracle = jData["metadata"]["viableComparisons"].get<long double>();
			long double pjOracle = jData["metadata"]["P(X)"].get<long double>();
			
			if(twoProportionsSampleRequirements(niOracle, piOracle, njOracle, pjOracle)){
				generalInclusionMatrix.set(i,j, false);
				return;
			} else {
				generalInclusionMatrix.set(i,j, true);
			}
			
			long double unpooledSE = twoProportionsUnpooledSE(
				iData["metadata"]["VAR(X)"].get<long double>(), niOracle, 
				jData["metadata"]["VAR(X)"].get<long double>(), njOracle
			);
				
			long double zStat = (piOracle - pjOracle) / unpooledSE;
			long double pValue =  erfc(abs(zStat) / sqrt(2.0)); 

			generalZMatrix.at(i,j) = zStat;
			generalZMatrix.at(j,i) = zStat;
			generalPMatrix.at(i,j) = pValue;
			generalPMatrix.at(j,i) = pValue;
			
			calcConfidenceInterval(piOracle, pjOracle, i, j, unpooledSE, generalCIMatrix);
		}
};
