#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <clang/Frontend/FrontendAction.h>
#include <string>
#include "clang/Tooling/Tooling.h"
#include "../instrumentation-mechanism/code-enchanter/CodeEnchanterAction.h"
#include "helper.hpp"
#include "json.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
	filesystem::path 			inputDirs, 
								outputPath, 
								mrkrFormatsPath,
								metadataResultsPath,
								inputDir, 
								outputDir;
						
	bool 						onCopy = false, 
								nameEncodedMarkers = false, 
								clangFormatMode = false;
						
	set<string> 				excludeFilesSet, 
								excludeDirsSet;

	int 						idCounter = 0;						
	json 						markerFormatCfg;
	
	vector<filesystem::path> 	pathQueue;
	vector<string> 				toolArgs = {
									"-xc", 						// Treat the input as C code, can sometimes use xc++ for enhanced recovery.
									"-std=c17", 				// Use a C standard, https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3176.pdf C17 stable bugfix release. Hangt echter meer af van gebruiksfrequentie.
									"-nostdinc",				// Don't search for system headers
									"-Wno-all",					// Suppress all warnings, W-prefix stands for warning.
								};
	
	void loadConfigs(){
		cout << "Importing JSON configs.\n";
	   	ifstream markerFormatsFile(mrkrFormatsPath);
	   	if (!markerFormatsFile.is_open()) {
	    	cerr << "Error: Could not open required config files, see the source json folder for details." << "\n";
	   		return;
	   	}
	   	try {
//			cout << "JSON: " << markerFormats.dump(2) << endl;
			markerFormatCfg = json::parse(markerFormatsFile);;
	   	} catch (const json::parse_error& e) {
	    	cerr << "Parse error: " << e.what() << "\n";
	    	return;
	  	}
	}
	
	void setupFileQueue(){
		cout << "Setup File Queue Start. \n";
		if(filesystem::is_regular_file(inputDirs)){
			pathQueue.push_back(inputDirs);
		} else if(filesystem::is_directory(inputDirs)){
			for (const auto& entry : filesystem::recursive_directory_iterator(inputDirs)) {
				if (entry.path().extension() == ".c") {
					if(
						excludeFilesSet.find(entry.path().filename().string()) != excludeFilesSet.end() ||
						is_subpathOfDirs(excludeDirsSet, entry.path())
					){
						cout << "Excluded: " << entry.path().string() << "\n";
					} else {
						cout << "Enqueued: " << entry.path() << "\n";
						pathQueue.push_back(entry.path());
					}					
				}
		    }
		}
	}
	
	/**
	 * 	Preprocesses files before instrumentation, which include:
	 * 	Fixing dangling statements from constructs by adding the compound statement node brackets where necessary, does not work perfectly and thus can break.
	 * 	FIXME: 
	 *		Fix format to be performed on the on the copy-over instead when required. Retain path and dir whilst doing so.
	 */
	void preprocessQueueItem(filesystem::path path2process){
		if(!clangFormatMode) {return;}
		string command = "clang-format -i --style=\"{InsertBraces: true, BasedOnStyle: LLVM, SortIncludes: false}\" " + path2process.string(); // als same dir input gefixt is dan deze weer aanpassen?
	    int status = system(command.c_str());
	}
	
	void processSingle(){
		cout 	<< "Process Single. \n";				
		preprocessQueueItem(inputDirs);
		// TODO: should take outputPath instead of inputPath after clang formatting to outputPath?
		auto codeString = file2string(inputDirs); 
		unique_ptr<ASTFrontendAction> action = std::make_unique<CodeEnchanterAction>(
			inputDirs, inputDir, outputPath, outputDir, markerFormatCfg, 
			idCounter, nameEncodedMarkers, clangFormatMode
		);
		clang::tooling::runToolOnCodeWithArgs(std::move(action), codeString, toolArgs, inputDirs.string());
	}
	
	void processQueue(){
		cout 	<< "Process Queue. \n"				
				<< "pathQueue items: " << pathQueue.size() << "\n";
		
		for(const auto& path2process : pathQueue){
			cout << "Processing: " << path2process << "\n";
			filesystem::path outPath = outputDir/ rel_outputPath(inputDir, path2process);
			if(outPath.empty()){outPath = inputDir;}
			preprocessQueueItem(path2process); //Doe dit voordat AST unit ptr is aangemaakt. Prepare file for marking, this includes fixing dangling statements
			auto codeString = file2string(path2process);
	
			unique_ptr<ASTFrontendAction> action = std::make_unique<CodeEnchanterAction>(
				path2process, inputDir, outputPath, outputDir,markerFormatCfg, 
				idCounter, nameEncodedMarkers, clangFormatMode
			);
			clang::tooling::runToolOnCodeWithArgs(std::move(action), codeString, toolArgs, path2process.string());
		}
	}
	
	/**
	 *  The created file will be placed at the output root.
	 * 	TODO: Change constant extraction mechanism
	 */
	void createMarkerDefinitionHeaderFile(){
		cout << "Start instrumentation header file creation." << "\n";
		string content = 
			markerFormatCfg["pragma_once"].get<string>() + 
			markerFormatCfg["noOptPragma"].get<string>() + 
			"\n";
		
		if(nameEncodedMarkers){
			// ...
		} else {
			content += 
				markerFormatCfg["noOptAttributes"].get<string>()	+ " " +
				markerFormatCfg["functionType"].get<string>() 		+ " " +
				markerFormatCfg["name"].get<string>() 				+
				markerFormatCfg["paramSignature"].get<string>()		+ "{" +
				markerFormatCfg["definition"].get<string>()			+ "};"+
				"\n"; 
		}
		
		string outFile = (outputDir/ (markerFormatCfg["definitionFileName"].get<string>() + ".h")).string(); // Open the file; if it exists, it will be overwritten.

	    error_code EC;
	    llvm::raw_fd_ostream out(outFile, EC, llvm::sys::fs::OF_None);
	    if (EC) {llvm::errs() << "Error opening file: " << EC.message() << "\n";}
	    out << content; // Write the string to the file and optionally flush to manually push buffer contents to the file (the destructor will close the file automatically).
	    out.flush();
		cout << "Instrumentation header file written to: " << outFile << "\n";
	}
	
	public:
	CmdTasker(
		string inputPath, 
		string outputPath, 
		string mrkrFormatsPath,
		string metadataResultsPath,
		bool onCopy, 
		bool nameEncodedMarkers, 
		bool formatMode, 
		set<string> exclFilesSet,
		set<string> excludeDirsSet
	): 
		inputDirs(inputPath), 
		outputPath(outputPath), 
		mrkrFormatsPath(mrkrFormatsPath), 
		metadataResultsPath(metadataResultsPath),
		onCopy(onCopy), 
		nameEncodedMarkers(nameEncodedMarkers), 
		clangFormatMode(formatMode), 
		excludeFilesSet(exclFilesSet),
		excludeDirsSet(excludeDirsSet)
		{
			if(outputPath.empty()){outputPath = inputPath;}
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
		if(onCopy && (outputPath  != inputDirs)){
			cout << "Performing onCopy procedure. \n";
			cout << "inputPath: " << inputDirs << "\n";
			cout << "outputPath: " << outputPath << "\n";
			filesystem::remove_all(outputPath);
			filesystem::copy(inputDirs, outputPath, filesystem::copy_options::recursive);
		}	
		if(filesystem::is_regular_file(inputDirs)){
			inputDir = inputDirs.has_parent_path() ? inputDirs.parent_path() : ".";
			outputDir = outputPath.has_parent_path() ? outputPath.parent_path() : ".";
		} else if(filesystem::is_directory(inputDirs)){
			inputDir= inputDirs;
			outputDir= outputPath;
		} else {
			cout << "Unknown pathtype \n";
			return false;
		}
		return true;
	}
	
	void insertInstrumentationEnhancement(){
		if(!preparePaths()){return;	}
		if(filesystem::is_regular_file(inputDirs)){
			processSingle();
		} else if(filesystem::is_directory(inputDirs)){
			setupFileQueue();
			processQueue();
		} else {
			cout << "Path doesnt exist" << "\n";
			return;
		}
		createMarkerDefinitionHeaderFile();
		cout << "Markers inserted: " << idCounter << "\n";
		fs::create_directory(metadataResultsPath.string());
		saveJsonToFile(
			metadataResultsPath.string() + outputPath.parent_path().filename().string(), 
			json::object({{"MarkersInserted", idCounter} })
		);
	}
};

