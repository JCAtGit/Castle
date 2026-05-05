#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include "../../../../../include/helper.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/Tooling/Tooling.h"
#include "../ast-mechanism/EvalVisitor.hpp"

using namespace nlohmann;
using namespace std;
using fn = std::function<void()>;

class CmdTasker{
	private:		
		string inputPath, oraclePath, outputPath, mode, mrkrFormatsPath;
		json markerFormats;
		
		fn processor;
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
			string fileOracleData = file2string(oraclePath);
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
		void processFolder(){}
		void processCustom(){}
		
		fn selectProcessor() {
	        if (mode == "file")   return [this]() -> void { processFile(); };
	        if (mode == "folder") return [this]() -> void { processFolder(); };
	        if (mode == "custom") return [this]() -> void { processCustom(); };
			cout << "invalid choice for processor";
	        return nullptr;
	    }
		
	public:
	CmdTasker(string inputPath, string oraclePath, string outputPath, string  mode, string  mrkrCfgPath): 
			inputPath(inputPath), 
			oraclePath(oraclePath),
			outputPath(outputPath),
			mode(mode),
			mrkrFormatsPath(mrkrCfgPath)
	{
		processor = selectProcessor();
		loadMarkerFormats();
		format = markerFormats["0"];
	}
		
	void startEvaluationProcesses(){
		cout << "Evaluation (mode, path): " << "(" << inputPath << ", " << inputPath << ")";
		processor();
	}
};
	