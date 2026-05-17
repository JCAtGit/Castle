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
#include "../ast-mechanism/DecomIndexVisitor.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath, oracleDir, outputPath, mrkrFormatsPath;
		json markerFormats, evalResults;
		json selectedFormat;

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
				"-Xclang",
		        "-frecovery-ast-type"
		    };
			std::unordered_map<std::string, FunctionDecl*> decomASTFnDeclIndex; 	// can be used to check marker integrity amongst other
			std::unordered_map<int, CallExpr*> decomASTCallExprIndex; 				// used for quick fetching of markers
			auto astDecomUnit = clang::tooling::buildASTFromCodeWithArgs(fileEvalData, args, inputPath.filename().c_str());
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args, oracleDir.string() + inputPath.filename().string());
			
			// TODO: identify markers and check integrity.
			auto decomIndexVisitor = make_unique<DecomIndexVisitor>(
				selectedFormat,
				astDecomUnit->getASTContext(), 
				decomASTFnDeclIndex, 
				decomASTCallExprIndex
			);
			
			cout << "testprint0: " << decomASTCallExprIndex.size() << endl;
			for(const auto &pair : decomASTCallExprIndex){
				cout << pair.first << ":" << pair.second->getDirectCallee()->getNameInfo().getName().getAsString() << endl;
			}
			
			auto evalVisitor = make_unique<EvalVisitor>(
				astOracleUnit->getASTContext(),
				astDecomUnit->getASTContext(), 
				selectedFormat,
				evalResults,
				decomASTFnDeclIndex,
				decomASTCallExprIndex
			);
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
		selectedFormat = markerFormats["0"];
	}
		
	void startEvaluationProcesses(){
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oracleDir.string() + inputPath.filename().string() << ")" << endl;
		processFile();
	}
};
	