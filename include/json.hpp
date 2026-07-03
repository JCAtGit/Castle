#pragma once
#include <filesystem>
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using namespace nlohmann;
using namespace std;

/**
 *	Generic method to load json filedata into json datastructure.
 *	@param	jsonFilePath
 *	@param	jsonRef
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
    	cerr << "loadJSON parse error: " << e.what() << endl;
    	return;
  	}
}

/**
 *	Generic method to save json data to file.
 *	@param	path	location data will be stored
 *	@param	data	data selected to be stored.
 */
void saveJsonToFile(const string& path, const json& data) {
    ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << path << "\n";
        return;
    }
    file << data.dump(2);  // pretty print with indentation
}

/**
 *	Increments a json entry, initializes with 1 if no value was present.
 */
void incrementJSONEntry(json& entry){
	if(entry.is_null()){
		entry = 1;
	} else {
		entry = entry.get<int>() + 1;
	}
}
