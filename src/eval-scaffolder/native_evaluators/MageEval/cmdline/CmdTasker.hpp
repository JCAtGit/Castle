#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "helper.hpp"
#include "../ast-mechanism/CrossASTEvaluator.hpp"
#include "../ast-mechanism/CrossASTGatherer.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath, oracleDir, outputPath, mrkrFormatsPath;
		json markerFormats, findings, findings_eval, dev;
		json selectedFormat;
		bool &nameEncodedMarkers;
		
		const vector<std::string> args = {
	        "-xc++", 	// xc++ or xc makes a huge difference for node recovery and is worth consideration
			"-w",
			"-std=c17", // voor c and cpp
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
		    	cerr << "MageEval: parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		void processDir(){
			unordered_map<string, unique_ptr<ASTUnit>> astOracleUnits;
			unordered_map<string, unique_ptr<ASTUnit>> astDecomUnits;
			string mapKey;
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
				if(entry.path().extension() == ".c" || entry.path().extension() == ""){
					fileOracleData = file2string(entry.path());
					mapKey = entry.path().filename().string();
					astOracleUnits[mapKey] = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + entry.path().filename().string());
					CAGatherer.setOracleContext(&(astOracleUnits[mapKey])->getASTContext());
					CAGatherer.TraverseDecl(astOracleUnits[mapKey]->getASTContext().getTranslationUnitDecl());
				}
 			}
			
			CAGatherer.selectMode(0);
			for(auto entry :filesystem::recursive_directory_iterator(inputPath)){ // inputPath is nu de dir waar de decompiled c zit
				// decompilers outputten soms zonder .c extension, maar aan de andere kant krijgen we ook regelmatig troep van bijvoorbeeld retdec.
				if(entry.path().extension() == ".c" || entry.path().extension().empty()){ 
					fileDecomData = file2string(entry.path());
					mapKey = entry.path().filename().string();
					astDecomUnits[mapKey] = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, entry.path().filename().string());
					CAGatherer.setDecomContext(&(astDecomUnits[mapKey])->getASTContext());
					CAGatherer.TraverseDecl(astDecomUnits[mapKey]->getASTContext().getTranslationUnitDecl());
					// TODO: consolidate findings here
					CAGatherer.announceFindings();
					cout << "Starting eval" << "\n";
					CrossASTEvaluator CAEvaluator(findings, findings_eval, dev, outputPath);
					
					CAEvaluator.startEval();
					
					cout << "Saving dir-entry result to json:" << outputPath << "\n";
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
			string fileDecomData = file2string(inputPath);
			string fileOracleData = file2string(oracleDir / inputPath.filename()); // TODO: oraclePath is an dir, which must be changed
			auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, inputPath.filename().c_str());
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + inputPath.filename().string());
			
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
			inputPath(inputPath), 		// file or dir
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
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oracleDir.string() + inputPath.filename().string() << ")" << "\n";
		if(filesystem::is_directory(inputPath)){// TODO: if it does not exist, then it will proceed to file procession as well
			processDir();
		} else {
			processFile();
		}		
	}
};
	