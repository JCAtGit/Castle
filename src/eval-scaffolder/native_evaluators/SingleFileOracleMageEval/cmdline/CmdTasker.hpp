#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include "../../../../../include/helper.hpp"
#include "../../SingleFileOracleMageEval/ast-mechanism/EvalVisitor.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/Tooling/Tooling.h"

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
			auto visitor = std::make_unique<EvalVisitor>(format);
			
			// TODO: complete, test drive first
			visitor->TraverseDecl(astEvalUnit->getASTContext().getTranslationUnitDecl());
		}
	
	public:
	CmdTasker(string inputPath, string oraclePath, string outputPath, string  mrkrFormatsPath): 
			inputPath(inputPath), 
			oraclePath(oraclePath),
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
	