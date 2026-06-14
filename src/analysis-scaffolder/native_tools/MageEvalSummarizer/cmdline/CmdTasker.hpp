#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "helper.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		filesystem::path inputPath;
		filesystem::path outputPath;	
		const string evalSuffix = "_eval.json", devSuffix = "_dev.json";
		
		
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path outputPath
	){

	}
	
	/**
	 * Benodigheden:
	 * 	- eval.json
	 *	- dev.json
	 *
	 * We analyseren de markers die we hebben kunnen vinden, markers die verloren zijn gegaan gaan in een aparte statistiek?
	 * Of beschouwen we deze ook als een missed recovery? We kunnen een statistiek "including missed" opnemen?
	 */
	void startProcesses(){
		cout << "Summarizing inputPath: " << inputPath << "\n";
			
	}
	
	// Process each finding element as separate set
	void processAsElements(){
		
	}
	
	// Process each finding element as part of the same set
	void processAsSet(){
		
	}
};
	