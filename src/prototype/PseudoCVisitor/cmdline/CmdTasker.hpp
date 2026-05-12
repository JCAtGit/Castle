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
		string inputPseudoCPath;			

	public:
	CmdTasker(string inputPath): 
			inputPseudoCPath(inputPseudoCPath)		// file or dir
	{}
	
		void processFile(){
			string fileData = file2string(inputPseudoCPath);
			std::vector<std::string> args = {
		        "-xc",
		        "-frecovery-ast",
		        "-frecovery-ast-type"
		    };
			auto astOracleUnit = clang::tooling::buildASTFromCodeWithArgs(fileData, args);

			auto testVisitor = std::make_unique<TestVisitor>(astOracleUnit->getSourceManager());
			testVisitor->TraverseDecl(astOracleUnit->getASTContext().getTranslationUnitDecl());
			// TODO: complete, test drive first
		}
	
		
};
	