#pragma once
// expand_macros.cpp
// Compile with: clang++ -std=c++17 expand_macros.cpp -o expand_macros \
//   `llvm-config --cxxflags --ldflags --libs --system-libs` -lclang-cpp

#include "clang/AST/ASTConsumer.h"
#include "clang/Analysis/MacroExpansionContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <vector>
#include <string>
#include <optional>

#include "../../../mage/preprocessor-mechanism/macro-expander/ast-mechanism/MacroExpanderAction.hpp"
#include "../../../mage/preprocessor-mechanism/macro-expander/MacroExpanderFactory.hpp"

using namespace clang;
using namespace clang::tooling;
using namespace std;

static llvm::cl::OptionCategory MacroExpanderCategory("macro-expander");

int runMacroExpander(string path2process, struct config* cfg) {
	std::vector<const char*> args;
	args.push_back("ms-macro-expander");               // argv[0] – program name
	args.push_back(path2process.c_str());      // argv[1] – source file
	
	int argc = args.size();
	const char** argv = args.data();
	
  	auto ExpectedParser = CommonOptionsParser::create(argc, argv, MacroExpanderCategory);
  	if (!ExpectedParser) {
    	llvm::errs() << ExpectedParser.takeError();
    	return 1;
  	}
  	CommonOptionsParser &OptionsParser = ExpectedParser.get();
  	ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
	MacroExpanderActionFactory MacroExpanderActionFactory(path2process, *cfg);
	
	llvm::outs() << "attempting macro expansion: " + path2process+ "\n";
  	return Tool.run(&MacroExpanderActionFactory);
}

