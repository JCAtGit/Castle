#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Tooling/Tooling.h"
#include "helper.hpp"
#include "../ast-mechanism/CrossASTEvaluator.hpp"
#include "../ast-mechanism/CrossASTGatherer.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputDirs, oracleDir, outputPath, mrkrFormatsPath;
		json markerFormats, findings, findings_eval, dev;
		json selectedFormat;
		bool &nameEncodedMarkers;
		
		const vector<std::string> args = {
	        "-xc++", 	// xc++ or xc makes a huge difference for node recovery and is worth consideration
			"-w",
//			"-std=c17", // voor c alleen en dus voor -xc flag
//			"-Xclang",
//	        "-frecovery-ast",
//			"-Xclang",
//	        "-frecovery-ast-type",
			"-ferror-limit=1",
	    };
		
		void loadMarkerFormats(){
			ifstream mrkrFormatsFile(mrkrFormatsPath);
			if (!mrkrFormatsFile.is_open()) {
			    	cerr << "Error: Could not open required marker formats file, see the source json folder for details." << endl;
			   		return;
			}
		   	try {
				markerFormats = json::parse(mrkrFormatsFile);
		   	} catch (const json::parse_error& e) {
		    	cerr << "MageEval parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		/**
		 *	TODO: 
		 *		Cleanup AST, parentStack when appropriate, currently enormous amounts of memory is used.
		 *		De unordered map is overgebleven van een andere implementatievorm, rework ooit.
		 *		Check of parentStack niet stuk gaat bij verwijderen map bij lifetime wijzigingen.
		 */
		void processDir(){
//			unordered_map<string, unique_ptr<ASTUnit>> astOracleUnits;
//			unordered_map<string, unique_ptr<ASTUnit>> astDecomUnits;
			string mapKey;
			// gatherer bevat geen verwijzing naar de maps
			CrossASTGatherer CAGatherer(
				selectedFormat,
				findings, 
				nameEncodedMarkers
			);
			cout<< "processDir" << "\n";
			string fileOracleData;
			string fileDecomData;

			
			CAGatherer.selectMode(1);
			for(auto entry :filesystem::recursive_directory_iterator(oracleDir)){ // voor elke moeten we apart een save to findings, etc maken.
				if(entry.path().extension() == ".c"){ // We nemen aan dat oracle dirs altijd gewoon C extensie gebruiken.
					fileOracleData = file2string(entry.path());
					mapKey = entry.path().filename().string();
//					astOracleUnits[mapKey] = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + entry.path().filename().string());
//					CAGatherer.setOracleContext(&(astOracleUnits[mapKey])->getASTContext());
//					CAGatherer.TraverseDecl(astOracleUnits[mapKey]->getASTContext().getTranslationUnitDecl());

					auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + entry.path().filename().string());
					CAGatherer.setOracleContext(&astOracleUnit->getASTContext());
					CAGatherer.TraverseDecl(astOracleUnit->getASTContext().getTranslationUnitDecl());
				}
 			}
			
			cout<< "gathering decomData" << "\n";
			CAGatherer.selectMode(0);
			for(auto entry :filesystem::recursive_directory_iterator(inputDirs)){ // inputDirs is nu de dir waar de decompiled c zit
				// Decompilers, zoals Ghidra, outputten soms zonder .c extensie, maar aan de andere kant krijgen we ook regelmatig troep van bijvoorbeeld retdec.
				// Hier moeten we dus bij de selectie een beetje de nodige aanpassingen maken te zijner tijd.
				if(entry.path().extension() == ".c" || entry.path().extension().empty()){ 
					fileDecomData = file2string(entry.path());
					mapKey = entry.path().filename().string();
					
//					astDecomUnits[mapKey] = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, entry.path().filename().string());
//					CAGatherer.setDecomContext(&(astDecomUnits[mapKey])->getASTContext());
//					CAGatherer.TraverseDecl(astDecomUnits[mapKey]->getASTContext().getTranslationUnitDecl());
					
					auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, entry.path().filename().string());
					CAGatherer.setDecomContext(&astDecomUnit->getASTContext());
					CAGatherer.TraverseDecl(astDecomUnit->getASTContext().getTranslationUnitDecl());				

					// TODO: consolidate findings here, de maps lijken hier niet meer nodig te zijn.
					CAGatherer.announceFindings();
					cout << "Starting eval: " << entry.path().string() <<"\n";
					CrossASTEvaluator CAEvaluator(findings, findings_eval, dev, outputPath);
					
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
		
		void processFile(){
			cout<< "processFile" << "\n";
			string fileDecomData = file2string(inputDirs);
			string fileOracleData = file2string(oracleDir / inputDirs.filename()); // TODO: oraclePath is an dir, which must be changed
			auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, inputDirs.filename().c_str());
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + inputDirs.filename().string());
			
			CrossASTGatherer CAGatherer(
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
			CrossASTEvaluator CAEvaluator(findings, findings_eval, dev, outputPath);
			CAEvaluator.startEval();
			
			cout << "Saving results to json:" << outputPath.parent_path().string() << "\n"; // outputPath is dir?
			
			filesystem::create_directory(outputPath.parent_path().string() + "/findings/");
			filesystem::create_directory(outputPath.parent_path().string() + "/eval/");
			filesystem::create_directory(outputPath.parent_path().string() + "/dev/");

			saveJsonToFile(outputPath.parent_path().string() + "/findings/" 	+ inputDirs.filename().string() + ".json", findings);
			saveJsonToFile(outputPath.parent_path().string() + "/eval/" 		+ inputDirs.filename().string() + ".json", findings_eval);
			saveJsonToFile(outputPath.parent_path().string() + "/dev/" 		+ inputDirs.filename().string() + ".json", dev);
		}
	
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path oracleDir, 
		filesystem::path outputPath, 
		filesystem::path mrkrFormatsPath,
		bool& nameEncodedMarkers
	): 
			inputDirs(inputPath), 		// file or dir
			oracleDir(oracleDir), 		// when iterating over files, looks for same name in oracle path
			outputPath(outputPath),
			mrkrFormatsPath(mrkrFormatsPath),
			nameEncodedMarkers(nameEncodedMarkers)
	{
		loadMarkerFormats();
		selectedFormat = markerFormats;
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
		
	void startProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputDirs << ", " << oracleDir.string() + inputDirs.filename().string() << ")" << "\n";
		if(filesystem::is_directory(inputDirs)){// TODO: if it does not exist, then it will proceed to file procession as well
			processDir();
		} else if (filesystem::is_regular_file(inputDirs)){
			processFile();
		} else {
			cout << "Path does not exist" << "\n";
		}
		cout << "startProcesses concluded \n";

	}
};
	