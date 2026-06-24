#pragma once
#include <string>
#include <filesystem>

using namespace std;

string getCastleFormatBuildNameFromAnalysisName(string analysisNameFormatString){
	if((analysisNameFormatString.at(0) == '_') || analysisNameFormatString.size() < 9) {throw "Error: getCastleFormatBuildNameFromAnalysisName";}
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
