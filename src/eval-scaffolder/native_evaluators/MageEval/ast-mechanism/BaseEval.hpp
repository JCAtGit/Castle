
#include <nlohmann/json.hpp>
#include <string>

using namespace nlohmann;
using namespace std;

const json GenEval = {
	{
		{"markee_construct"		, "NULL"},
		{"recovered_construct"	, false}
	}
};

const unordered_map<string, json> EvalMap = {
	{"CallExpr", {
		{"parity_params_count"	, false},
		{"parity_args_count"	, false},
		{"parity_variadicity"	, false},
		{"parity_side_effects"	, false},
		{"parity_direct_callee"	, false}
	}},
	{"DeclStmt", {
		
	}},
	{"IfStmt", {
		
	}},
	{"ForStmt", {
		
	}},
	{"WhileStmt", {
		
	}},
	{"DoStmt", {
		
	}},
	{"BinaryOperator", {
		
	}}
};

