#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <clang/Frontend/FrontendAction.h>
#include "../../../../../include/helper.hpp"
#include "../instrumentation-mechanism/marker-inserter/CodeMarkerAction.h"
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
	
	/**
	 * 	This file will be used to link separately, so that an optimization barrier can be created.
	 *  The created file will be placed at the output root.
	 *  Check the preview folder for an example output.
	 TODO: Change constant extraction mechanism
	 */
	void createMarkerDefinitionHeaderFile(struct config& cfg){
		llvm::outs() << "Start marker header file creation." << "\n";
		string content = 
			inclusionDirectiveIFNDEF + " " + inclusionDirectiveInclGuard + "\n" +
			inclusionDirectiveDEF + " " + inclusionDirectiveInclGuard + "\n" +
			cfg.markerFunctionSignatureString + '\n' + 
			inclusionDirectiveENDIF;
					
		// Open the file; if it exists, it will be overwritten.
	    error_code EC;
	    llvm::raw_fd_ostream out((cfg.outputDir / (cfg.markerDefinitionFileName + ".h")).string(), EC, llvm::sys::fs::OF_None);
	    if (EC) {llvm::errs() << "Error opening file: " << EC.message() << "\n";}

	    // Write the string to the file and optionally flush (the destructor will close the file automatically).
	    out << content;
	    out.flush();
		llvm::outs() << "Marker header file written. \n";
	}
	
	void createMarkerDefinitionSourceFile(struct config& cfg){
		llvm::outs() << "Start marker source file creation." << "\n";
		string content = 
			headerInclusionDirective + "\"" + (cfg.markerDefinitionFileName + ".h") + "\"\n" +
			cfg.markerDefinitionString;
		
		// Open the file; if it exists, it will be overwritten.
	    error_code EC;
	    llvm::raw_fd_ostream out((cfg.outputDir / (cfg.markerDefinitionFileName + ".c")).string(), EC, llvm::sys::fs::OF_None);
	    if (EC) {llvm::errs() << "Error opening file: " << EC.message() << "\n";}

	    // Write the string to the file and optionally flush (the destructor will close the file automatically).
	    out << content;
	    out.flush();
		llvm::outs() << "Marker source file written. \n";
	}
	
	void wrapUpQueue(){
		
	}
	
	
	
	
	public:
	CmdTasker(string inputPath, string outputPath, string mrkrFormatsPath): inputPath(inputPath), outputPath(outputPath), mrkrFormatsPath(mrkrFormatsPath) {
		loadConfigs();
	}
	
	/**
	 * 	Should accept only the following:
	 *		inputPath	| outputPath| 	valid:
	 *		file		| file		|	true, 	convert to virtual dir (dir -> dir) with 1 file.
	 *		file		| dir		| 	false, 	throw exception
	 *		dir			| file		|	false,	throw exception
	 *		dir			| dir		|	true
	 */
	bool preparePaths(bool onCopy){
		if(onCopy){
			std::filesystem::remove_all(outputPath);
			std::filesystem::copy(inputPath, outputPath);
			inputPath = outputPath;
		}	
		if(filesystem::is_regular_file(inputPath) && filesystem::is_regular_file(outputPath)){
			inputDir = inputPath.parent_path();
			outputDir = outputPath.parent_path();
		} else if(filesystem::is_directory(inputPath) && filesystem::is_directory(outputPath)){
			inputDir = inputPath;
			outputDir = outputPath;
		} else return false;
		return true;
	}
	
	void insertInstrumentationEnhancement(){
		setupFileQueue();
		processQueue();
		wrapUpQueue();
	}
	
	void removeInstrumentationEnhancement(){
		// create dependency files
	}
	
};

