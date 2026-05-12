#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "../../../../../include/helper.hpp"
#include "../ast-mechanism/EvalVisitor.hpp"
#include "../ast-mechanism/TestVisitor.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		string inputPath, oraclePath, outputPath, mrkrFormatsPath;
		json markerFormats;
		
		json format;
		
		void loadMarkerFormats(){
			ifstream mrkrFormatsFile(mrkrFormatsPath);
			if (!mrkrFormatsFile.is_open()) {
			    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
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
			string fileOracleData = file2string(oraclePath); // TODO: oraclePath is an dir, which must be changed
			std::vector<std::string> args = {
		        "-xc",
		        "-frecovery-ast",
		        "-frecovery-ast-type"
		    };
			auto astEvalUnit = clang::tooling::buildASTFromCodeWithArgs(fileEvalData, args);
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args);
			
			// first identify markers and check integrity.
			
//			auto visitor = std::make_unique<EvalVisitor>(format);
//			visitor->TraverseDecl(astEvalUnit->getASTContext().getTranslationUnitDecl());

			auto testVisitor = std::make_unique<TestVisitor>(astOracleUnit->getSourceManager(), format);
			testVisitor->TraverseDecl(astOracleUnit->getASTContext().getTranslationUnitDecl());
			// TODO: complete, test drive first
		}
	
	public:
	CmdTasker(string inputPath, string oraclePath, string outputPath, string  mrkrFormatsPath): 
			inputPath(inputPath), 		// file or dir
			oraclePath(oraclePath), 	// when iterating over files, looks for same name in oracle path
			outputPath(outputPath),
			mrkrFormatsPath(mrkrFormatsPath)
	{
		loadMarkerFormats();
		format = markerFormats["0"];
	}
		
	void startEvaluationProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oraclePath << ")";
		processFile();
	}
};
	