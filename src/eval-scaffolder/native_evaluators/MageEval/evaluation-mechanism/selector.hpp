#pragma once

#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include <nlohmann/json.hpp>
#include "clang/AST/RecursiveASTVisitor.h"

using namespace nlohmann;
using namespace clang;
using namespace std;

void getSiblings(){
	
}

void getNextSibling(){
	
}

void startEval(CallExpr* CE, SourceManager& oracleSM, SourceManager& decomSM,  unordered_map<string, clang::FunctionDecl*>& decomASTIndex, json& evalResults){
	oracleSM
}

