#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "helper.hpp"
#include "json.hpp"
#include "../analysis/MageEvalAnalyst.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputDirs;
		filesystem::path outputPath;

		int processMode;
		int it_begin, it_end;
		
		string findingsFolder = "findings/", evalFolder = "eval/", devFolder = "dev/";
		
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path outputPath,
		int processMode, 
		int it_begin, 
		int it_end
	) : 
	inputDirs(inputPath), 
	outputPath(outputPath), 
	processMode(processMode), 
	it_begin(it_begin), 
	it_end(it_end)
	{
		findingsFolder 	= inputPath.string() + findingsFolder;
		evalFolder 		= inputPath.string() + evalFolder;
		devFolder 		= inputPath.string() + devFolder;
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
		cout << "Analyzing inputPath: " << inputDirs << "\n";
		
		json currentJSON, summaryJSON = json::object();
		MageEvalAnalyst MEA;
		filesystem::create_directories(outputPath.string());

		switch (processMode) {
			default:
				break;
			case 0:{
				for(auto entry :filesystem::recursive_directory_iterator(findingsFolder)){
//					cout << "chk0:" << entry.path().string() << "\n";
					loadJSON(entry, currentJSON); // use the names in this dir to fetch related data in corelated directories
					MEA.statisticizeFindings(currentJSON, summaryJSON);
					
//					cout << "chk1:" << evalFolder + entry.path().filename().string() << "\n";
					loadJSON(evalFolder + entry.path().filename().string(), currentJSON);
					MEA.statisticizeEvals(currentJSON, summaryJSON);

//					cout << "chk2:" << devFolder + entry.path().filename().string() << "\n";
					loadJSON(devFolder + entry.path().filename().string(), currentJSON);
					MEA.statisticizeDevReports(currentJSON, summaryJSON);
				}
				
				MEA.summaryStochasticPass(summaryJSON);
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
					
					MEA.summaryStochasticPass(summaryJSON);
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
					
					MEA.summaryStochasticPass(summaryJSON);
					saveJsonToFile(outputPath.string() + filename ,summaryJSON);
					summaryJSON.clear();
				}
				break;
			}
		}
		
	}
};
	