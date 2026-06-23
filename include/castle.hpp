#pragma once
#include <string>
#include <filesystem>

using namespace std;


// Does not account for relative constructs such as ../ or ./, just absolute paths.
filesystem::path getTopLvlDirFromAbsPath(filesystem::path somePath){
	if(somePath.is_absolute()){
		
	} else throw "Path is not absolute";
}


// Als deze heel vaak uitgevoerd wordt, dan kunnen we beter een toegewijde TRIE-datastructuur implementeren.
void getBinaryPathsFromTConfigName(string &buildScaffolderDir ,string & AnalysisTConfigName){
	// First we trim until the build dir substring
	string buildPathSubstr = AnalysisTConfigName.substr();

}