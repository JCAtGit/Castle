#pragma once
#include <string>

using namespace std;

/**
 *	Castle formats each name within the pipeline with a predetermined format.
 *	Format: codebase_instrumentation_build_decom_eval_analysis.
 *		Example A: 
 *			csmith_mage_clang_ghidra, an intermediate format. It stems from the codebase csmith, which then has been altered with mage. 
 *
 *		Example B: 
 *			If given some name with format; codebase_instrumentation_build_decom_eval_analysis
 			The function will return; 	 codebase_instrumentation_build
 *			Else returns the same string.
 *		Then compilation takes place with clang and decompilation with ghidra.
 *		This intermediate product can form preceding link of "csmith_mage_clang_ghidra_mageEval_mageEvalAnalyzerIterated" for example.
 *	
 *	@param 	analysisNameFormatString	name representing some castle format string.
 *	@return	The build-phase format of the string
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

///**
// *	Castle formats each name within the pipeline with a predetermined format.
// *	Format: codebase_instrumentation_build_decom_eval_analysis.
// *		Example A: 
// *			csmith_mage, an intermediate format. It stems from the codebase csmith, which then has been altered with mage. 
// *
// *		Example B: 
// *			If given some name with format; codebase_instrumentation_build_decom_eval_analysis
// 			The function will return; 	 codebase_instrumentation
// *			Else returns the same string.
// *	
// *	@param 	analysisNameFormatString name representing some castle format string.
// *	@return	The mage-phase format of the string
// */
//string getCastleFormatInstrNameFromAnalysisName(string analysisNameFormatString){
//	if((analysisNameFormatString.at(0) == '_') || analysisNameFormatString.size() < 9) {
//		throw "Error: getCastleFormatBuildNameFromAnalysisName, targets should follow the codebase_instrumentation_build_decom_eval_analysis name format.";
//	}
//	int cnt = 0;
//	for(int i=1;i<analysisNameFormatString.length();i++){
//		if(analysisNameFormatString.at(i) == '_'){
//			cnt++;
//			if (cnt == 3){
//				return analysisNameFormatString.substr(0, i);
//			}
//		}
//	}
//	return analysisNameFormatString;
//}

/**
 *	Castle formats each name within the pipeline with a predetermined format.
 *	Format: codebase_instrumentation_build_decom_eval_analysis.
 *		Example A: 
 *			csmith_mage, an intermediate format. It stems from the codebase csmith, which then has been altered with mage. 
 *
 *		Example B: 
 *			If given some name with format; codebase_instrumentation_build
 *			The function will return; codebase_instrumentation
 *			Else returns the same string.
 *	
 *	@param 	analysisNameFormatString name representing some castle format string.
 *	@return	The mage-phase format of the string (i.e., codebase_instrumentation)
 */
string getCastleFormatInstrNameFromBuildName(string analysisNameFormatString){
    // Basic sanity check: should not start with '_' and have minimal length
    if ((analysisNameFormatString.at(0) == '_') || analysisNameFormatString.size() < 9) {
        throw "Error: getCastleFormatInstrNameFromBuildName, targets should follow the codebase_instrumentation_build name format.";
    }
    int cnt = 0;
    for (int i = 1; i < analysisNameFormatString.length(); i++) {
        if (analysisNameFormatString.at(i) == '_') {
            cnt++;
            // Return after the second underscore → "codebase_instrumentation"
            if (cnt == 2) {
                return analysisNameFormatString.substr(0, i);
            }
        }
    }
    // If not enough underscores, return the string unchanged
    return analysisNameFormatString;
}
