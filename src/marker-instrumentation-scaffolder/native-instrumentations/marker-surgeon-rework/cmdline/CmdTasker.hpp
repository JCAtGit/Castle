#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <clang/Frontend/FrontendAction.h>
#include "../../../../../include/helper.hpp"
#include "../instrumentation-mechanism/marker-inserter/ast-mechanism/CodeMarkerAction.h"
#include "../instrumentation-mechanism/marker-remover/ast-mechanism/CodeRemoverAction.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
	filesystem::path inputPath, outputPath, mrkrFormatsPath;
	json markerFormats; 
	
	json markerFormatCfg;
	filesystem::path inputFile;
	filesystem::path inputDir;
	filesystem::path outputDir;
	std::vector<filesystem::path> pathQueue;
	int taskQuantifier = 0;
	
	void loadConfigs(){
		cout << "Importing JSON configs.\n";
	   	ifstream markerFormatsFile(mrkrFormatsPath);
	   	if (!markerFormatsFile.is_open()) {
	    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
	   		return;
	   	}
	   	try {
	    	markerFormats = json::parse(markerFormatsFile);
			markerFormatCfg = markerFormats["0"];
	   	} catch (const json::parse_error& e) {
	    	cerr << "Parse error: " << e.what() << endl;
	    	return;
	  	}
	}
	
	/**
	 * 	Should accept only the following:
	 *		inputPath	| outputPath| 	valid:
	 *		file		| file		|	true, 	convert to virtual dir (dir -> dir) with 1 file.
	 *		file		| dir		| 	false, 	throw exception
	 *		dir			| file		|	false,	throw exception
	 *		dir			| dir		|	true
	 */
	void preparePaths(){
		if(filesystem::is_regular_file(inputPath) && filesystem::is_regular_file(outputPath)){
			inputDir = inputPath.parent_path();
			outputDir = outputPath.parent_path();
		} else if(filesystem::is_directory(inputPath) && filesystem::is_directory(outputPath)){
			inputDir = inputPath;
			outputDir = outputPath;
		} else {return;}
		std::filesystem::remove_all(outputPath);
		std::filesystem::copy(inputPath, outputPath);
	}
	
	void setupFileQueue(){
		cout << "Setup File Queue Start. \n";
		if(filesystem::is_regular_file(inputPath)){
			pathQueue.push_back(inputPath);
		} else if(filesystem::is_directory(inputPath)){
			if(taskQuantifier == 1){
				for (const auto& entry : filesystem::directory_iterator(inputPath)) {
					if (entry.path().extension() == ".c") {
						cout << "Enqueued: " << entry.path() << "\n";
						pathQueue.push_back(entry.path());
					}
			    }
			} else {
				for (const auto& entry : filesystem::recursive_directory_iterator(inputPath)) {
					if (entry.path().extension() == ".c") {
						cout << "Enqueued: " << entry.path() << "\n";
						pathQueue.push_back(entry.path());
					}
			    }
			}
		}
	}
	
	/**
	 * Preprocesses files before marking, which include:
	 * Fixing dangling statements from constructs by adding the compound statement node brackets where necessary.
	 * TODO: Pre-expand macros?
	 */
	void preprocessQueueItem(filesystem::path path2process, filesystem::path outputResultPath){
		std::string command;
		int status;
		//"clang-format --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + "example0.c" + ">" + "example0_formatted.c";
		command = "clang-format -i --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + path2process.string() + ">" + outputResultPath.string();
	    status = std::system(command.c_str());
	    if (status == 0) {
			cout << "clang-format performed for:" + path2process.string() + "\n";
	    } else {
			cout << "clang-format failed for:" + path2process.string() + "\n";
	    }
//				runMacroExpander(path2process, cfg);
	}
	
	void processQueue(){
		cout 	<< "Process Queue Start. \n"				
				<< "pathQueue items: " << pathQueue.size() << "\n";
		for(const auto& path2process : pathQueue){
			cout << "Starting on " << path2process << "\n" ;
			filesystem::path outPath = outputDir / rel_outputPath(inputDir, path2process);
	
			//Doe dit voordat AST unit ptr is aangemaakt. Prepare file for marking, this includes fixing dangling statements
			preprocessQueueItem(path2process, outPath);
			
			auto codeString = file2string(path2process);
			unique_ptr<ASTFrontendAction> action;
	
			// TODO?: extract selection of action
			cout << "Enhancing " << path2process.string() << "\n";
			action = std::make_unique<CodeMarkerAction>(markerFormatCfg);
			}
			clang::tooling::runToolOnCodeWithArgs(std::move(action), codeString, cfg->ToolArgs, path2process.string());
		}
	}
	
	void insertInstrumentationEnhancement(){
		setupFileQueue();
		processQueue();
	}
	
	void removeInstrumentationEnhancement(){
		
	}
	
	
	
	public:
	CmdTasker(string inputPath, string outputPath, string mrkrFormatsPath): inputPath(inputPath), outputPath(outputPath), mrkrFormatsPath(mrkrFormatsPath) {
		loadConfigs();
	}
};

