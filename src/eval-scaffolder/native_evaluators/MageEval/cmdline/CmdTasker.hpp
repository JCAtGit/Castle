#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "../../../../../include/helper.hpp"
#include "../../MageEval/ast-mechanism/EvalVisitor.hpp"
#include "../../MageEval/ast-mechanism/TestVisitor.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath, oracleDir, outputPath, mrkrFormatsPath;
		json markerFormats, evalResults;
		json format;

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
			string fileEvalData = file2string(inputPath);
			string fileOracleData = file2string(oracleDir); // TODO: oraclePath is an dir, which must be changed
			vector<std::string> args = {
		        "-xc",
				"-w",
				"-Xclang",
		        "-frecovery-ast",
		        "-frecovery-ast-type"
		    };
			auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileEvalData, args, inputPath.filename().c_str());
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + inputPath.filename().string());

			// first identify markers and check integrity.
			auto evalVisitor = make_unique<EvalVisitor>(
				astOracleUnit->getASTContext(),
				astDecomUnit->getASTContext(), 
				format,
				evalResults);
			evalVisitor->TraverseDecl(astDecomUnit->getASTContext().getTranslationUnitDecl());
		}
	
	public:
	CmdTasker(filesystem::path inputPath, filesystem::path oracleDir, filesystem::path outputPath, filesystem::path  mrkrFormatsPath): 
			inputPath(inputPath), 		// file or dir
			oracleDir(oracleDir), 		// when iterating over files, looks for same name in oracle path
			outputPath(outputPath),
			mrkrFormatsPath(mrkrFormatsPath)
	{
		loadMarkerFormats();
		format = markerFormats["0"];
	}
		
	void startEvaluationProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oracleDir.string() + inputPath.filename().string() << ")" << endl;
		processFile();
	}
};
	