#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <clang/Frontend/FrontendAction.h>
#include "clang/Tooling/Tooling.h"
#include "../../../../../include/helper.hpp"
#include "../instrumentation-mechanism/code-enchanter/CodeEnchanterAction.h"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
	filesystem::path inputPath, outputPath, mrkrFormatsPath;
	bool onCopy = false;
	json markerFormats; 
	
	json markerFormatCfg;
	filesystem::path inputDir;
	filesystem::path outputDir;
	std::vector<filesystem::path> pathQueue;
	int taskQuantifier = 0;
	std::vector<std::string> toolArgs = {
		"-xc", 							// Treat the input as C code
		"-std=c11", 					// Use a specific C standard
		"-nostdinc",					// Don't search for system headers
//      "-D__STDC__=1",						//
		"-Wno-all",						// Suppress all warnings, W-prefix stands for warning.
	};
	
	void loadConfigs(){
		cout << "Importing JSON configs.\n";
	   	ifstream markerFormatsFile(mrkrFormatsPath);
	   	if (!markerFormatsFile.is_open()) {
	    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
	   		return;
	   	}
	   	try {
	    	markerFormats = json::parse(markerFormatsFile);
//			cout << "JSON: " << markerFormats.dump(4) << endl;
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
						cout << "Enqueued: " << entry.path() << endl;
						pathQueue.push_back(entry.path());
					}
			    }
			} else {
				for (const auto& entry : filesystem::recursive_directory_iterator(inputPath)) {
					if (entry.path().extension() == ".c") {
						cout << "Enqueued: " << entry.path() << endl;
						pathQueue.push_back(entry.path());
					}
			    }
			}
		}
	}
	
	/**
	 * Preprocesses files before instrumentation, which include:
	 * Fixing dangling statements from constructs by adding the compound statement node brackets where necessary.
	 * TODO: Pre-expand macros?
	 */
	void preprocessQueueItem(filesystem::path path2process, filesystem::path outputResultPath){
		//"clang-format --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + "example0.c" + ">" + "example0_formatted.c";
		std::string command = "clang-format --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + path2process.string() + " > " + outputResultPath.string();
//		cout << command << endl;
	    int status = std::system(command.c_str());
//	    if (status == 0) {
//			cout << "clang-format performed for:" + path2process.string() << endl;
//	    } else {
//			cout << "clang-format failed for:" + path2process.string() << endl;
//	    }
//				runMacroExpander(path2process, cfg);
	}
	
	void processSingle(){
		cout 	<< "Process Single Start. \n"				
				<< "pathQueue items: " << pathQueue.size() << endl;
				
		preprocessQueueItem(inputPath, outputPath);
		auto codeString = file2string(outputPath); // TODO: should take outputPath instead of inputPath after clang formatting to outputPath
//		cout << "Enchanting " << inputPath.string() << endl;
		unique_ptr<ASTFrontendAction> action = std::make_unique<CodeMarkerAction>(inputPath, inputPath, inputDir, outputPath, outputDir,markerFormatCfg);
		clang::tooling::runToolOnCodeWithArgs(std::move(action), codeString, toolArgs, inputPath.string());
	}
	
	void processQueue(){
		cout 	<< "Process Queue Start. \n"				
				<< "pathQueue items: " << pathQueue.size() << endl;
		
		for(const auto& path2process : pathQueue){
			cout << "Starting on " << path2process << endl;
			filesystem::path outPath = outputDir / rel_outputPath(inputDir, path2process);
	
			//Doe dit voordat AST unit ptr is aangemaakt. Prepare file for marking, this includes fixing dangling statements
			preprocessQueueItem(path2process, outPath);
			auto codeString = file2string(path2process);
	
			// TODO?: extract selection of action
//			cout << "Enchanting " << path2process.string() << endl;
			unique_ptr<ASTFrontendAction> action = std::make_unique<CodeMarkerAction>(path2process, inputPath, inputDir, outputPath, outputDir,markerFormatCfg);
			clang::tooling::runToolOnCodeWithArgs(std::move(action), codeString, toolArgs, path2process.string());
		}
	}
	
	/**
	 *  The created file will be placed at the output root.
	 *  Check the preview folder for an example output.
	 * 	TODO: Change constant extraction mechanism
	 */
	void createMarkerDefinitionHeaderFile(){
		cout << "Start instrumentation header file creation." << endl;
		string content = markerFormatCfg["headerString"].get<string>();
		string outFile = (outputDir / (markerFormatCfg["definitionFileName"].get<string>() + ".h")).string();
		// Open the file; if it exists, it will be overwritten.
	    error_code EC;
	    llvm::raw_fd_ostream out(outFile, EC, llvm::sys::fs::OF_None);
	    if (EC) {llvm::errs() << "Error opening file: " << EC.message() << "\n";}

	    // Write the string to the file and optionally flush (the destructor will close the file automatically).
	    out << content;
	    out.flush();
		cout << "Instrumentation header file written to: " << outFile << endl;
	}
	
	public:
	CmdTasker(string inputPath, string outputPath, string mrkrFormatsPath, bool onCopy): 
		inputPath(inputPath), outputPath(outputPath), mrkrFormatsPath(mrkrFormatsPath), onCopy(onCopy) {
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
	bool preparePaths(){
		cout << "Preparing paths. \n";
		if(onCopy && (outputPath != inputPath)){
			cout << "Performing onCopy procedure. \n";
			std::cout << "inputPath: " << inputPath << "\n";
			std::cout << "outputPath: " << outputPath << "\n";
			filesystem::remove_all(outputPath);
			filesystem::copy(inputPath, outputPath);
			inputPath = outputPath;
		}	
		if(filesystem::is_regular_file(inputPath)){
			inputDir = inputPath.has_parent_path() ? inputPath.parent_path() : ".";
			outputDir = outputPath.has_parent_path() ? outputPath.parent_path() : ".";
		} else if(filesystem::is_directory(inputPath)){
			inputDir = inputPath;
			outputDir = outputPath;
		} else {
			return false;
		}
		return true;
	}
	
	void insertInstrumentationEnhancement(){
		if(!preparePaths()){
			return;
		}
		if(filesystem::is_regular_file(inputPath)){
			processSingle();
		} else {
			setupFileQueue();
			processQueue();
		}
		createMarkerDefinitionHeaderFile();
	}
	
	void removeInstrumentationEnhancement(){
		// create dependency files
		// if a single file is given we just cleanse the file
		if(filesystem::is_regular_file(inputPath)){
//			unique_ptr<ASTFrontendAction> action = std::make_unique<CodeMarkerRemover>(inputPath, inputDir, outputPath, outputDir,markerFormatCfg);
		} else {
//			for (const auto& entry : filesystem::recursive_directory_iterator(inputPath)) {
//				if (entry.path().extension() == ".c") {
//					cout << "Enqueued: " << entry.path() << endl;
//					pathQueue.push_back(entry.path());
//				} else if(entry.path().filename() == markerFormatCfg["definitionFileName"].get<string>()){
//					filesystem::remove(entry.path());
//				}
//		    }
		}

		// if a dir is given we try to cleanse the dir recursively and remove any marker headers files we come across
	}
	
};

