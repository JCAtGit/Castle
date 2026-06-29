#pragma once
#include <filesystem>
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using namespace nlohmann;
using namespace std;

/**
 *	Generic method to load json filedata in json datastructure.
 */
void loadJSON(filesystem::path jsonFilePath, json& jsonRef){
	ifstream jsonFile(jsonFilePath);
	if (!jsonFile.is_open()) {
	    	cerr << "Error: Could not open required marker formats file, see the source json folder for details." << endl;
	   		return;
	}
   	try {
		jsonRef = json::parse(jsonFile);
   	} catch (const json::parse_error& e) {
    	cerr << "MageEvalComparer parse error: " << e.what() << endl;
    	return;
  	}
}

/**
 *	Generic method to save json data to file.
 */
void saveJsonToFile(const string& path, const json& data) {
    ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << path << "\n";
        return;
    }
    file << data.dump(2);  // pretty print with indentation
	cout << "Saved config to " << path << "\n";
}
