#pragma once
#include <string>
using namespace std;
	
	const string pragma_once = "#pragma once\n";
	const string headerInclusionDirective = "#include ";
	const string static_inline_type_modifier = "static inline";
	const string inclusionDirectiveInclGuard = "CODEMARKER_ALPHA"; // hardcoded for now
	const string inclusionDirectiveIFNDEF = "#ifndef";
	const string inclusionDirectiveDEF = "#define";
	const string inclusionDirectiveENDIF = "#endif";
	
	const string noinlineAttribute = "	__attribute__((noinline, used, nothrow, noipa))";
	const string memBarrier = "__asm__ volatile(\"\" ::: \"memory\");\n";