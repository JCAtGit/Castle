#pragma once
#include <string>

using namespace std;

/**
 *	Castle formats each name within the pipeline with a predetermined format.
 *	Namely: codebase_instrumentation_build_decom_eval_analysis.
 *		Example A: csmith_mage_clang_ghidra
 *		Example A shows an intermediate result stemming. It stems from the codebase csmith, which then has been altered with mage. 
 *		Then compilation takes place with clang and decompilation with ghidra.
 *		This intermediate product can form preceding link of "csmith_mage_clang_ghidra_mageEval_mageEvalAnalyzerIterated" for example.
 */
string getCastleFormatBuildNameFromAnalysisName(string analysisNameFormatString){
	if((analysisNameFormatString.at(0) == '_') || analysisNameFormatString.size() < 9) {
		throw "Error: getCastleFormatBuildNameFromAnalysisName, targets should follow the codebase_instrumentation_build_decom_eval_analysis name format.";
	}
	int cnt = 0;
	for(int i=1;i<analysisNameFormatString.length();i++){
		if(analysisNameFormatString.at(i) == '_'){
			cnt++;
			if (cnt == 3){
				return analysisNameFormatString.substr(0, i);
			}
		}
	}
	return analysisNameFormatString;
}
