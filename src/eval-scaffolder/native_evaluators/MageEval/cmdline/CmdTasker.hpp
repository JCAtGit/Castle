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
		string inputPath, oraclePath, outputPath, mrkrFormatsPath;
		json markerFormats;
		json format;
		std::unordered_map<std::string, clang::FunctionDecl*> decomFnDeclIndex; // of gewoon een algemene decl index?

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
		
		// Build index from AST: map qualified name -> FunctionDecl*
	    void indexASTFunctionDecls(std::unordered_map<std::string, clang::FunctionDecl*> &astFnDeclIndex, clang::ASTContext &context) {
	        for (auto *D : context.getTranslationUnitDecl()->decls()) {
	            if (auto *FD = llvm::dyn_cast<clang::FunctionDecl>(D)) {
	                std::string name = FD->getQualifiedNameAsString();
	                astFnDeclIndex[name] = FD;
	            }
	        }
	    }
		
		void processFile(){
			string fileEvalData = file2string(inputPath);
			string fileOracleData = file2string(oraclePath); // TODO: oraclePath is an dir, which must be changed
			std::vector<std::string> args = {
		        "-xc",
				"-Xclang",
		        "-frecovery-ast",
				"-Xclang",
		        "-frecovery-ast-type"
		    };
			auto astEvalUnit = clang::tooling::buildASTFromCodeWithArgs(fileEvalData, args);
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileOracleData, args);
			indexASTFunctionDecls(decomFnDeclIndex, astEvalUnit->getASTContext());

			// first identify markers and check integrity.
			auto evalVisitor = std::make_unique<EvalVisitor>(astOracleUnit->getSourceManager(), format, astEvalUnit->getASTContext());
			evalVisitor->TraverseDecl(astEvalUnit->getASTContext().getTranslationUnitDecl());

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
		cout << "Evaluation (inputPath, oraclePath): " << "(" << inputPath << ", " << oraclePath << ")" << endl;
		processFile();
	}
};
	