#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include "paths.hpp"
#include "matrix.hpp"
#include "stats.hpp"
#include <charconv>
#include <string_view>
#include "binaryFingerPrinter.hpp"

using namespace nlohmann;
using namespace std;

bool parseDashCouplingString(std::string_view sv, int &a, int &b) {
    const char* start = sv.data();
    const char* end = start + sv.size();

    // Parse a
    auto [ptr_a, ec_a] = std::from_chars(start, end, a);
    if (ec_a != std::errc() || ptr_a == end) return false;

    // Skip the hyphen
    if (*ptr_a != '-') return false;
    ++ptr_a;

    // Parse b
    auto [ptr_b, ec_b] = std::from_chars(ptr_a, end, b);
    return ec_b == std::errc();
}


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
	appendRadareBinaryCharacteristicsConstructs(
	CSVData, 
	inputDirs,
	outputPath, 
	summaryData,
	binaryMetadata,
	contextKey
	);
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
	appendRadareBinaryCharacteristicsGeneral(
		CSVData, 
		inputDirs,
		outputPath, 
		summaryData,
		binaryMetadata,
		contextKey
	);
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

void appendHolmBonferroniScores(
	string &CSVData, 
	vector<filesystem::path> &inputDirs,
	vector<pair<string, long double>> &hbPVector,
	vector<long double> &hbValueVector,
	Matrix<string> &CIMatrix,
	ostringstream &oss
){
	int r, c;
	CSVData += "\nHolmBonferroniScores\n";
	CSVData += "H,P,HolmThreshold,Decision,Confidence Interval\n";
	for(int entry=0;entry<hbPVector.size();entry++){
		oss << hbPVector[entry].second;
		CSVData += hbPVector[entry].first + "," + oss.str() + ",";
		oss.clear();
		oss.str("");
		oss << hbValueVector[entry];
		CSVData += oss.str();
		oss.clear();
		oss.str("");
		if(hbPVector[entry].second <= hbValueVector[entry]){
			CSVData += ",reject";
		} else {
			CSVData += ",stop";
		}
		// unpooled Wald interval
		parseDashCouplingString(hbPVector[entry].first, r, c);
		CSVData += "," + CIMatrix.at(r, c);
		CSVData += "\n";
	}
	CSVData += "\n";
}

