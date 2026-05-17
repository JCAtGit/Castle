#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "../../../../../include/helper.hpp"
#include "../ast-mechanism/DecomIndexHandler.hpp"
#include "../ast-mechanism/DecomIndexVisitor.hpp"

#include "../ast-mechanism/EvalVisitor.hpp"

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

			// Set up MatchFinder
		    DecomIndexHandler handler(selectedFormat, decomASTFnDeclIndex, decomASTCallExprIndex);
		    MatchFinder finder;
		    finder.addMatcher(functionDecl().bind("func"), &handler);
		    finder.addMatcher(callExpr().bind("call"), &handler);
		    
		    // Run matching – this will populate the indices and also build parent maps
		    finder.matchAST(astDecomUnit->getASTContext());

//			auto decomIndexVisitor = make_unique<DecomIndexVisitor>(
//				selectedFormat,
//				astDecomUnit->getASTContext(),
//				decomASTFnDeclIndex,
//				decomASTCallExprIndex
//			);

		    // Now the maps are filled. The parent maps are also available.
		    std::cout << "testprint0: " << decomASTCallExprIndex.size() << std::endl;
		    for (const auto &pair : decomASTCallExprIndex) {
		        std::cout << pair.first << ":" << pair.second->getDirectCallee()->getNameInfo().getName().getAsString() << std::endl;
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
	