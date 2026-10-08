#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "../../../native-tools/MageEvalAnalyzer/analysis/MageEvalAnalyst.hpp"
#include "paths.hpp"

#include "json.hpp"

using namespace nlohmann;
using namespace std;

/**
 *	Schedules the necessary tasks for analysis.
 */
class CmdTasker{
	private:		
		filesystem::path 	inputDirs, 
							outputPath;

		int 				processMode,
							it_begin, it_end;
		
		string 				findingsFolder = "findings/", evalFolder = "eval/", devFolder = "dev/",
							itEntryExt = ".c.json", summaryFilename = "summary.json",
							metadataResultsPath;
							
		json 				currentJSON, 	// Container structure with data from: findings, eval and optionally dev.
							summaryJSON = json::object(),
							metadataJSON;
							
		MageEvalAnalyst 	MEA; 			// init with default constructor
		
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path outputPath,
		int processMode, 
		int it_begin, 
		int it_end,
		string metadataResultsPath
	) : 
	inputDirs(inputPath), 
	outputPath(outputPath), 
	processMode(processMode), 
	it_begin(it_begin), 
	it_end(it_end),
	metadataResultsPath(metadataResultsPath)
	{
		findingsFolder 	= inputPath.string() + findingsFolder;
		evalFolder 		= inputPath.string() + evalFolder;
		devFolder 		= inputPath.string() + devFolder;
	}
	
	/**
	 *	Entry function, starts performing the analysis.
	 */
	void startProcesses(){
		cout << "Analyzing inputPath: " << inputDirs << "\n";
		filesystem::create_directories(outputPath.string());
		switch (processMode) {
			default:break;
			case 0: processDirAsSet(); 						break;
			case 1: processAsSeparateEntities(); 			break;
			case 2: processAsSeparateIterationEntities(); 	break;
		}
	}
	
	/**
	 *	Treats the dir contents as related entities, thus summarizing its findings as a collective.
	 * 	@param MEA			Performs the analysis and summarization.
	 * 	@param currentJSON	Structure containing data from: findings, eval and optionally dev.
	 *	@param summaryJSON	Structure containing end results.
	 */
	void processDirAsSet(){
		for(auto entry :filesystem::recursive_directory_iterator(findingsFolder)){
			loadJSON(entry, currentJSON); // use the names in this dir to fetch related data in corelated directories
			MEA.statisticizeFindings(currentJSON, summaryJSON);
			
			loadJSON(evalFolder + entry.path().filename().string(), currentJSON);
			MEA.statisticizeEvals(currentJSON, summaryJSON);

			loadJSON(devFolder + entry.path().filename().string(), currentJSON);
			MEA.statisticizeDevReports(currentJSON, summaryJSON);
		}
		
		MEA.summaryStochasticPass(summaryJSON);
		saveJsonToFile(outputPath.string() + summaryFilename ,summaryJSON);
	}
	
	/**
	 *	Treats each file in the dir as its own separate entity, producing summaries for each one of them.
	 * 	@param MEA			Performs the analysis and summarization.
	 * 	@param currentJSON	Structure containing data from: findings, eval and optionally dev.
	 *	@param summaryJSON	Structure containing end results.
	 */
	void processAsSeparateEntities(){
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
	}
	
	/**
	 *	Treats each file in the dir as its own separate entity, producing summaries for each one of them.
	 * 	Processes only the files designated within the index range, using an index namestem.
	 * 	@param MEA			Performs the analysis and summarization.
	 * 	@param currentJSON	Structure containing data from: findings, eval and optionally dev.
	 *	@param summaryJSON	Structure containing end results.
	 */
	void processAsSeparateIterationEntities(){
		string filename;
		for(;it_begin<it_end;it_begin++){
			filename = to_string(it_begin )+ itEntryExt;
			loadJSON(findingsFolder + filename, currentJSON); // use the names in this dir to fetch related data in related directories
			MEA.statisticizeFindings(currentJSON, summaryJSON);
			
			loadJSON(evalFolder + filename, currentJSON);
			MEA.statisticizeEvals(currentJSON, summaryJSON);

			loadJSON(devFolder + filename, currentJSON);
			MEA.statisticizeDevReports(currentJSON, summaryJSON);
			
			MEA.summaryStochasticPass(summaryJSON);
			saveJsonToFile(outputPath.string() + filename ,summaryJSON);
			summaryJSON.clear();
		}
	}
};
	