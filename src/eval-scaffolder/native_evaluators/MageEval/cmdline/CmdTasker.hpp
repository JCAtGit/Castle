#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "paths.hpp"
#include "../ast-mechanism/ContextGatherer.hpp"

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Tooling/Tooling.h"
#include "json.hpp"
#include "../ast-mechanism/CrossContextEvaluator.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path 		inputPath, 
								oracleDir, 
								outputPath, 
								mrkrFormatsPath;
							
		json 					markerFormats, 
								findings, 
								findings_eval, 
								dev,
								selectedFormat;
							
		bool 					&nameEncodedMarkers;
		
		const vector<string> 	args = {
							        "-xc++", 			// xc++ or xc makes a huge difference for node recovery and is worth considering for C files as well
									"-w",
						//			"-std=c17", 			// voor c alleen en dus voor -xc flag
									"-ferror-limit=1",
							    };
		
		void loadMarkerFormats(){
			ifstream mrkrFormatsFile(mrkrFormatsPath);
			if (!mrkrFormatsFile.is_open()) {
			    	cerr << "Error: Could not open required marker formats file, see the source json folder for details.\n";
			   		return;
			}
		   	try {
				markerFormats = json::parse(mrkrFormatsFile);
		   	} catch (const json::parse_error& e) {
		    	cerr << "MageEval parse error: " << e.what() << "\n";
		    	return;
		  	}
		}
		
		/**
		 *	Directory processing function.
		 *	If a directory is given as input target, then it will cross evaluate each source file inside the directory against the full oracle map.
		 */
		void processDir(){
			cout<< "processDir" << "\n";
			string mapKey;
			ContextGatherer CAGatherer(
				selectedFormat,
				findings, 
				nameEncodedMarkers
			);
			string fileOracleData;
			string fileDecomData;
			
			CAGatherer.selectMode(1);
			for(auto entry :filesystem::recursive_directory_iterator(oracleDir)){ // voor elke apart een save to findings, etc maken.
				if(entry.path().extension() == ".c"){ // We nemen aan dat oracle sources altijd C extensie gebruikt.
					fileOracleData = file2string(entry.path());
					auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(
						fileOracleData, 
						args, 
						oracleDir.string() + entry.path().filename().string());
					CAGatherer.setOracleContext(&astOracleUnit->getASTContext());
					CAGatherer.TraverseDecl(astOracleUnit->getASTContext().getTranslationUnitDecl());
				}
 			}
			
			cout<< "gathering decomData" << "\n";
			CAGatherer.selectMode(0);
			for(auto entry :filesystem::recursive_directory_iterator(inputPath)){ // inputDirs is nu de dir waar de decompiled c zit
				// Decompilers, outputten soms met verschillende extensies ook krijgen we ook regelmatig ongewenstheden van bijvoorbeeld retdec.
				// Hier moeten we dus bij de selectie de nodige aanpassingen maken te zijner tijd. (of iets anders bedenken)
				if(entry.path().extension() == ".c" || entry.path().extension().empty()){ 
					fileDecomData = file2string(entry.path());
					mapKey = entry.path().filename().string();
					
					auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, entry.path().filename().string());
					CAGatherer.setDecomContext(&astDecomUnit->getASTContext());
					CAGatherer.TraverseDecl(astDecomUnit->getASTContext().getTranslationUnitDecl());				

					CAGatherer.announceFindings();
					cout << "Starting eval: " << entry.path().string() <<"\n";
					CrossContextEvaluator CAEvaluator(findings, findings_eval, dev, outputPath);
					
					CAEvaluator.startEval();
					cout << "Saving result of " << mapKey << " in " << outputPath << "\n";
					filesystem::create_directory(outputPath.string() + "findings/");
					filesystem::create_directory(outputPath.string() + "eval/");
					filesystem::create_directory(outputPath.string() + "dev/");
					
					saveJsonToFile(outputPath.string() + "findings/" 	+ mapKey + ".json", findings);
					saveJsonToFile(outputPath.string() + "eval/" 		+ mapKey + ".json", findings_eval);
					saveJsonToFile(outputPath.string() + "dev/" 		+ mapKey + ".json", dev);
					
					CAEvaluator.clearEval();
					findings_eval.clear();
					dev.clear();
				}
			}
		}
		
		/**
		 *	File processing function.
		 *	If a file is given as input target, then it will evaluate this single file against the full oracle map.
		 */
		void processFile(){
			cout<< "processFile" << "\n";
			string fileDecomData = file2string(inputPath);
			string fileOracleData = file2string(oracleDir / inputPath.filename()); // TODO: oraclePath is an dir, which must be changed
			auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, inputPath.filename().c_str());
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + inputPath.filename().string());
			
			ContextGatherer CAGatherer(
				selectedFormat,
				findings, 
				&(astOracleUnit->getASTContext()), 
				&(astDecomUnit->getASTContext()),
				nameEncodedMarkers
			);
	
			CAGatherer.selectMode(0);
			CAGatherer.TraverseDecl(astDecomUnit->getASTContext().getTranslationUnitDecl());
			CAGatherer.selectMode(1);
			CAGatherer.TraverseDecl(astOracleUnit->getASTContext().getTranslationUnitDecl());
			CAGatherer.announceFindings();
			
			cout << "Starting eval" << "\n";
			CrossContextEvaluator CAEvaluator(findings, findings_eval, dev, outputPath);
			CAEvaluator.startEval();
			
			cout << "Committing results to json:" << outputPath.parent_path().string() << "\n"; // outputPath is dir?
			filesystem::create_directory(outputPath.parent_path().string() + "/findings/");
			filesystem::create_directory(outputPath.parent_path().string() + "/eval/");
			filesystem::create_directory(outputPath.parent_path().string() + "/dev/");

			saveJsonToFile(outputPath.parent_path().string() + "/findings/" 	+ inputPath.filename().string() + ".json", findings);
			saveJsonToFile(outputPath.parent_path().string() + "/eval/" 		+ inputPath.filename().string() + ".json", findings_eval);
			saveJsonToFile(outputPath.parent_path().string() + "/dev/" 		+ inputPath.filename().string() + ".json", dev);
		}
	
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path oracleDir, 
		filesystem::path outputPath, 
		filesystem::path mrkrFormatsPath,
		bool& nameEncodedMarkers
	): 
			inputPath(inputPath), 					// file or dir
			oracleDir(oracleDir), 					// when iterating over files, looks for same name in oracle path
			outputPath(outputPath),
			mrkrFormatsPath(mrkrFormatsPath),
			nameEncodedMarkers(nameEncodedMarkers)
	{
		loadMarkerFormats();
		selectedFormat = markerFormats;
	}
	
	/**
	 *	Entry function, starts the rudimentary evaluation processes.
	 */
	void startProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oracleDir.string() + inputPath.filename().string() << ")" << "\n";
		if(!filesystem::exists(inputPath)){
			cout << "Path does not exist" << "\n";
			return;
		}
		if(filesystem::is_directory(inputPath)){
			processDir();
		} else if (filesystem::is_regular_file(inputPath)){
			processFile();
		} 
		cout << "startProcesses concluded \n";
	}
};
	