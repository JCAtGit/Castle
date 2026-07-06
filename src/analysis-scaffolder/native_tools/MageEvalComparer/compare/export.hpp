#pragma once
#include "nlohmann/json.hpp"
#include <string>

#include "paths.hpp"
#include "matrix.hpp"
#include "stats.hpp"

using namespace nlohmann;
using namespace std;

// Keys to determine which binary metadata to pull from radare2 output.
const vector<string> radare2Keys = {
	"arch",  "bits", "bintype", "class", "compiler", "endian", "lang", "machine", "os"
};

/**
 *	Appends single csv row containing a label.
 */
void appendLabelRow(string &CSVData, string label){
	CSVData += label + "\n";
}

void appendConstructMappingOnly(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	const string& outputPath, 
	vector<json> &summaryData,
	const string& contextKey
){
	for(int i = 0; i<inputDirs.size();i++){
		int vcCount = 0;
		if(summaryData[i]["constructs"].contains(contextKey)){
			vcCount = summaryData[i]["constructs"][contextKey]["viableComparisons"].get<int>();
		}
		CSVData += 
			to_string(i) 																			+ "," + 
			get_leaf_string(inputDirs[i].string()) 												+ "," +
			to_string(vcCount);
			CSVData += "\n";
	}
	CSVData += "\n";
}

void appendGeneralMappingOnly(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	const string& outputPath, 
	vector<json> &summaryData,
	const string& contextKey
){
	for(int i = 0; i<inputDirs.size();i++){
		int vcCount = summaryData[i]["metadata"]["viableComparisons"].get<int>();
		CSVData += 
			to_string(i) 																			+ "," + 
			get_leaf_string(inputDirs[i].string()) 												+ "," +
			to_string(vcCount);
			CSVData += "\n";
	}
	CSVData += "\n";
}

/**
 *	
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 */
void appendBinaryCharacteristicsConstructs(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	const string& outputPath, 
	vector<json> &summaryData,
	vector<json> &binaryMetadata,
	const string& contextKey
){
	// Display general characteristics
	CSVData = "EntryMapping," + contextKey + ",viableComparisons";

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
}

/**
 *	
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 */
void appendBinaryCharacteristicsGeneral(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	const string& outputPath, 
	vector<json> &summaryData,
	vector<json> &binaryMetadata,
	const string &contextKey
){
	CSVData = "EntryMapping," + contextKey + ",viableComparisons";
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
}

/**
 *	
 *	@param
 *	@param
 *	@param
 *	@param
 */
template <typename T>
void appendZScores(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	Matrix<T> &contextZData, 
	ostringstream &oss
){
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
}

/**
 *	
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 */
template <typename T>
void appendPValues(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	Matrix<T> &contextPData, 
	ostringstream &oss
){
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
}

/**
 *	
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 */
template <typename T>
void exportZConstructMatrixToCSV(
	vector<filesystem::path> &inputDirs,
	const string& outputPath, 
	vector<json> &summaryData,
	vector<json> &binaryMetadata,
	const string& contextKey,
	Matrix<T>& contextZData, 
	Matrix<T>& contextPData,
	bool &recombine_only
){
	ostringstream oss;
	oss << scientific << setprecision(3);
	string CSVData;
	
	if(recombine_only){
		appendConstructMappingOnly(CSVData, inputDirs, outputPath, summaryData, contextKey);
	} else {
		appendBinaryCharacteristicsGeneral(CSVData, inputDirs, outputPath, summaryData, binaryMetadata, contextKey);
	}	
	appendZScores(CSVData, inputDirs, contextZData, oss);
	appendPValues(CSVData, inputDirs, contextPData, oss);
	string2file(CSVData, outputPath);
}

/**
 *	Exports the data for the general criteria to csv format.
 */
template <typename T>
void exportZGeneralMatrixToCSV(
	vector<filesystem::path> &inputDirs,
	const string& outputPath, 
	vector<json> &summaryData,
	vector<json> &binaryMetadata,
	const string& contextKey,
	Matrix<T>& contextZData, 
	Matrix<T>& contextPData,
	bool &recombine_only
){
	ostringstream oss;
	oss << scientific << setprecision(3);
	
	string CSVData;
	if(recombine_only){
		appendConstructMappingOnly(CSVData, inputDirs, outputPath, summaryData, contextKey);
	} else {
		appendBinaryCharacteristicsGeneral(CSVData, inputDirs, outputPath, summaryData, binaryMetadata, contextKey);
	}
	appendZScores(CSVData, inputDirs, contextZData, oss);
	appendPValues(CSVData, inputDirs, contextPData, oss);
	string2file(CSVData, outputPath);
}