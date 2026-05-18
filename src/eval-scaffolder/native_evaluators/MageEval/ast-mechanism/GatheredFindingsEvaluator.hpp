#pragma once

#include <unordered_map>
#include <string>
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "nlohmann/json.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace nlohmann;

class GatheredFindingsEvaluator {
	public:
		GatheredFindingsEvaluator(){}
		
	private:
	
};
