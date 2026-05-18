#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "../../../../../include/helper.hpp"

#include "../ast-mechanism/DecomFindingsGatherer.hpp"
#include "../ast-mechanism/OracleFindingsGatherer.hpp"
#include "../ast-mechanism/GatheredFindingsEvaluator.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath, oracleDir, outputPath, mrkrFormatsPath;
		json markerFormats, evalResults;
		json selectedFormat;

		const vector<std::string> args = {
	        "-xc",
			"-w",
			"-Xclang",
	        "-frecovery-ast",
			"-Xclang",
	        "-frecovery-ast-type"
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
		    	cerr << "Parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		void processFile(){
			string fileDecomData = file2string(inputPath);
			string fileOracleData = file2string(oracleDir / inputPath.filename()); // TODO: oraclePath is an dir, which must be changed
			auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileDecomData, args, inputPath.filename().c_str());
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + inputPath.filename().string());
			unordered_map<int, json> decomFindings;
			unordered_map<int, json> oracleFindings;
			unordered_map<int, json> evalResults;
			DecomFindingsGatherer dfg(selectedFormat, astDecomUnit->getASTContext());
			OracleFindingsGatherer ofg(selectedFormat, astOracleUnit->getASTContext());
			GatheredFindingsEvaluator gfe;			
			
			// Gebruik andere actions voor FunctionDecls
		    MatchFinder decomFinder, oracleFinder;
			cout << "DecomMatcher" << endl;
		    decomFinder.addMatcher(callExpr().bind("dfgCallExpr"), &dfg);
			decomFinder.matchAST(astDecomUnit->getASTContext());

			cout << "OracleMatcher" << endl;
			oracleFinder.addMatcher(callExpr().bind("ofgCallExpr"), &ofg);
			oracleFinder.matchAST(astOracleUnit->getASTContext());
		    // Run matching – this will populate the indices and also build parent maps

		}
	
	public:
	CmdTasker(filesystem::path inputPath, filesystem::path oracleDir, filesystem::path outputPath, filesystem::path  mrkrFormatsPath): 
			inputPath(inputPath), 		// file or dir
			oracleDir(oracleDir), 		// when iterating over files, looks for same name in oracle path
			outputPath(outputPath),
			mrkrFormatsPath(mrkrFormatsPath)
	{
		loadMarkerFormats();
		selectedFormat = markerFormats["0"];
	}
		
	void startEvaluationProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oracleDir.string() + inputPath.filename().string() << ")" << endl;
		processFile();
	}
};
	