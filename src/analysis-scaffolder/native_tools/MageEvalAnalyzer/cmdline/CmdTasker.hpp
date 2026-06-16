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
#include "../analysis/MageEvalAnalyst.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath;
		filesystem::path outputPath;

		int processMode, analysisMode;
		int it_begin, it_end;
		string findingsFolder = "findings/", evalFolder = "eval/", devFolder = "dev/";
		
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path outputPath,
		int processMode, 
		int analysisMode,
		int it_begin, 
		int it_end
	) : inputPath(inputPath), outputPath(outputPath), processMode(processMode), analysisMode(analysisMode), it_begin(it_begin), it_end(it_end)
	{
		findingsFolder 	= inputPath.string() + findingsFolder;
		evalFolder 		= inputPath.string() + evalFolder;
		devFolder 		= inputPath.string() + devFolder;
	}
	
	void loadJSON(filesystem::path jsonFilePath, json& jsonRef){
		ifstream jsonFile(jsonFilePath);
		if (!jsonFile.is_open()) {
		    	cerr << "Error: Could not open required marker formats file, see the source json folder for details." << endl;
		   		return;
		}
	   	try {
			jsonRef = json::parse(jsonFile);
//			cout << jsonRef.dump(2) << "\n";
	   	} catch (const json::parse_error& e) {
	    	cerr << "MageEvalAnalyzer parse error: " << e.what() << endl;
	    	return;
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
		cout << "Analyzing inputPath: " << inputPath << "\n";
		
		json currentJSON, summaryJSON = json::object();
		MageEvalAnalyst MEA;
		filesystem::create_directories(outputPath.string());
		filesystem::create_directories(outputPath.string());
		filesystem::create_directories(outputPath.string());

		switch (processMode) {
			default:
				break;
			case 0:{
				for(auto entry :filesystem::recursive_directory_iterator(findingsFolder)){
					cout << "chk1:" << entry.path().string() << "\n";

					loadJSON(entry, currentJSON); // use the names in this dir to fetch related data in corelated directories
					MEA.statisticizeFindings(currentJSON, summaryJSON);
					
					loadJSON(evalFolder + entry.path().filename().string(), currentJSON);
					MEA.statisticizeEvals(currentJSON, summaryJSON);

					loadJSON(devFolder + entry.path().filename().string(), currentJSON);
					MEA.statisticizeDevReports(currentJSON, summaryJSON);
				}
				saveJsonToFile(outputPath.string() + "summary.json" ,summaryJSON);
				break;
			}
			case 1:{
				for(auto entry :filesystem::recursive_directory_iterator(findingsFolder)){
					loadJSON(entry, currentJSON); // use the names in this dir to fetch related data in corelated directories
					MEA.statisticizeFindings(currentJSON, summaryJSON);
					
					loadJSON(evalFolder + entry.path().filename().string(), currentJSON);
					MEA.statisticizeEvals(currentJSON, summaryJSON);

					loadJSON(devFolder + entry.path().filename().string(), currentJSON);
					MEA.statisticizeDevReports(currentJSON, summaryJSON);
					saveJsonToFile(outputPath.string() + entry.path().filename().string() ,summaryJSON);
					summaryJSON.clear();
				}
				break;
			}
			case 2:{
				// TODO: is a bit hardcoded, but meh
				string filename;
				for(;it_begin<it_end;it_begin++){
					filename = to_string(it_begin )+ ".c.json";
					loadJSON(findingsFolder + filename, currentJSON); // use the names in this dir to fetch related data in corelated directories
					MEA.statisticizeFindings(currentJSON, summaryJSON);
					
					loadJSON(evalFolder + filename, currentJSON);
					MEA.statisticizeEvals(currentJSON, summaryJSON);

					loadJSON(devFolder + filename, currentJSON);
					MEA.statisticizeDevReports(currentJSON, summaryJSON);
					saveJsonToFile(outputPath.string() + filename ,summaryJSON);
					summaryJSON.clear();
				}
				break;
			}
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
	