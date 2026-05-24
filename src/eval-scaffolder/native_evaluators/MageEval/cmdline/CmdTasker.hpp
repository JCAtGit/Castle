#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "../../../../../include/helper.hpp"
#include "../ast-mechanism/DecomFindingsGatherer.hpp"
#include "../ast-mechanism/OracleFindingsGatherer.hpp"
#include "../ast-mechanism/GatheredFindingsEvaluator.hpp"
#include "../pre-eval/CrossASTGatherer.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath, oracleDir, outputPath, mrkrFormatsPath;
		json markerFormats, findings, specEval, markerEval, humanEval;
		json selectedFormat;

		const vector<std::string> args = {
	        "-xc",
			"-w",
			"-Xclang",
	        "-frecovery-ast",
			"-Xclang",
	        "-frecovery-ast-type",
			"-ferror-limit=1"
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
//		    MatchFinder decomFinder, oracleFinder;
//			cout << "DecomMatcher" << endl;
//		    decomFinder.addMatcher(callExpr().bind("dfgCallExpr"), &dfg);
//			decomFinder.matchAST(astDecomUnit->getASTContext());
//
//			cout << "OracleMatcher" << endl;
//			oracleFinder.addMatcher(callExpr().bind("ofgCallExpr"), &ofg);
//			oracleFinder.matchAST(astOracleUnit->getASTContext());

			CrossASTGatherer CAVisitor(selectedFormat,findings, specEval, markerEval, humanEval, astOracleUnit->getASTContext(), astDecomUnit->getASTContext());
			CAVisitor.selectMode(0);
			CAVisitor.TraverseDecl(astDecomUnit->getASTContext().getTranslationUnitDecl());
			CAVisitor.selectMode(1);
			CAVisitor.TraverseDecl(astOracleUnit->getASTContext().getTranslationUnitDecl());
			CAVisitor.evalProcessor();
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
	
	void saveJsonToFile(const std::string& path, const json& data) {
	    std::ofstream file(path);
	    if (!file.is_open()) {
	        std::cerr << "Error: Could not open file for writing: " << path << "\n";
	        return;
	    }
	    file << data.dump(4);  // pretty print with 4 spaces indentation
	    std::cout << "Saved to "<<  path << "\n" << data.dump(4) << "\n";
	}
		
	void startEvaluationProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oracleDir.string() + inputPath.filename().string() << ")" << "\n";
		processFile();
		cout << "Saving results to json" << "\n";
		saveJsonToFile(outputPath.string() + ".json", findings);
		cout << "Starting eval" << "\n";
		
	}
};
	