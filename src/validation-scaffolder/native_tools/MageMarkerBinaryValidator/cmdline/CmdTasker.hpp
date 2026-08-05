#include <cstddef>
#include <filesystem>
#include "binaries.hpp"
#include "castle.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "paths.hpp"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"


#include "json.hpp"
#include "../validate/MarkerCounterVisitor.hpp"
#include "validation.hpp"

using namespace nlohmann;
using namespace std;

/**
 *	Schedules the necessary tasks for analysis.
 */
class CmdTasker{
	private:		
		filesystem::path 	inputBuildDir, 
							outputPath;

		int 				processMode,
							it_begin, it_end;
		bool&				nameEncodedMarkers;
		string 				summaryFilename = "summary.json",
		 					mrkrFormatsPath;
		json 				markerFormat;
		
		vector<string> 		toolArgs = {
								"-xc", 						// Treat the input as C code, can sometimes use xc++ for enhanced recovery.
								"-std=c17", 				// Use a C standard, https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3176.pdf C17 stable bugfix release. Hangt echter meer af van gebruiksfrequentie.
								"-nostdinc",				// Don't search for system headers
								"-Wno-all",					// Suppress all warnings, W-prefix stands for warning.
							};
		
	public:
	CmdTasker(
		filesystem::path inputBuildDir, 
		filesystem::path outputPath,
		bool& nameEncodedMarkers,
		int processMode, 
		int it_begin, 
		int it_end
	): 
	inputBuildDir(inputBuildDir), 
	outputPath(outputPath), 
	nameEncodedMarkers(nameEncodedMarkers),
	processMode(processMode), 
	it_begin(it_begin), 
	it_end(it_end)
	{
		loadJSON(mrkrFormatsPath, markerFormat);
		//... 
	}
	
	/**
	 *	For each file create a marker summary listing, if a file exists in both instr and build. Then we create a local summary, else we dont.
	 * 	We always create a general summary over the complete dir.
	 */
	void startProcesses(){
		cout << "Analyzing inputPath: " << inputBuildDir << "\n";
		filesystem::create_directories(outputPath.string());
		switch (processMode) {
			default:break;
			case 0: processDirAsSet(); break;
			case 1: processAsSeparateEntities(); break;
			case 2: processAsSeparateIterationEntities(); break;
		}
	}
	
	
	void processDirAsSet(){
		//iteration mode
		string filedata;
		json countJSON, resultJSON;
		auto instrDir = getCastleFormatInstrNameFromBuildName(inputBuildDir);
		string cmdDataString;
		MarkerCounterVisitor mcv(
			markerFormat,
			countJSON,
			nameEncodedMarkers
		);
		
		// iterations are files
		for(const auto& entry : filesystem::directory_iterator(instrDir)){
			if(entry.path().extension() == ".c"){
				filedata = file2string(entry.path());
				auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs( 
					filedata, 
					toolArgs, 
					entry.path().filename().c_str()
				);
				mcv.TraverseDecl(astDecomUnit->getASTContext().getTranslationUnitDecl());
			}
		}
		
		for(const auto& entry : filesystem::directory_iterator(inputBuildDir)){
			execCommandAndCaptureOutput("objdump -d 0 | grep mage_alpha", cmdDataString);
			validateMarkersFromObjdump(countJSON, cmdDataString);
		}
		
		for(auto [key, value] : countJSON.items()){
			if(value){
				incrementJSONEntry(resultJSON["present"]);
			} else {
				incrementJSONEntry(resultJSON["missing"]);
			}
		}
		
		saveJsonToFile(outputPath / "presence.json", resultJSON);
	}
	
	void processAsSeparateEntities(){}
	
	void processAsSeparateIterationEntities(){}
};


	