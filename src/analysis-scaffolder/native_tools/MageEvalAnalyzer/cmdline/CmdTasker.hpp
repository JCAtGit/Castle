#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "helper.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputDir;
		filesystem::path outputDir;
		int processMode, analysisMode;
		string findingsFolder = "findings", evalFolder = "eval", devFolder = "dev";
		
	public:
	CmdTasker(
		filesystem::path inputDir, 
		filesystem::path outputDir,
		int processMode, 
		int analysisMode
	) : inputDir(inputDir), outputDir(outputDir), processMode(processMode), analysisMode(analysisMode)
	{
		findingsFolder 	= inputDir.string() + findingsFolder;
		evalFolder 		= inputDir.string() + evalFolder;
		devFolder 		= inputDir.string() + devFolder;
	}
	
	json loadJSON(filesystem::path jsonFilePath){
		ifstream jsonFile(jsonFilePath);
		if (!jsonFile.is_open()) {
		    	cerr << "Error: Could not open required marker formats file, see the source json folder for details." << endl;
		   		return {};
		}
	   	try {
			return json::parse(jsonFile);
	   	} catch (const json::parse_error& e) {
	    	cerr << "MageEvalAnalyzer parse error: " << e.what() << endl;
	    	return {};
	  	}
	}
	
	/**
	 * Benodigheden mappen met JSON resultaten:
	 *	- findings
	 * 	- eval
	 *	- dev
	 *
	 * We analyseren de markers die we hebben kunnen vinden, markers die verloren zijn gegaan gaan in een aparte statistiek?
	 * Of beschouwen we deze ook als een missed recovery? We kunnen een statistiek "including missed" opnemen?
	 */
	void startProcesses(){
		cout << "Analyzing inputDir: " << inputDir << "\n";
		
		json currentJSON;
		for(auto entry :filesystem::recursive_directory_iterator(findingsFolder)){
			
		}
		for(auto entry :filesystem::recursive_directory_iterator(evalFolder)){
			
		}
		for(auto entry :filesystem::recursive_directory_iterator(devFolder)){
					
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
};
	