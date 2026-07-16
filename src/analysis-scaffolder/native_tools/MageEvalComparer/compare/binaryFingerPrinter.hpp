#pragma once
#include "binaries.hpp"
#include "castle.hpp"
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
 *	
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 *	@param
 */
void appendRadareBinaryCharacteristicsConstructs(
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
void appendRadareBinaryCharacteristicsGeneral(
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


