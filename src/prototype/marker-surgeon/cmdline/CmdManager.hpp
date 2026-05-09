#pragma once
#include "llvm/Support/raw_ostream.h"
#include <filesystem>
//#include "nlohmann/json.hpp"
#include <string>
#include <bits/stdc++.h>

#include "../../../../prototype/marker-surgeon/config/config_ds.hpp"
namespace fs = std::filesystem;

#include <CLI/CLI.hpp>


/**
 * 	TODO:
 * 		- Verbeter cmd parsing, gebruik CLI/CLI header
 * 	Default:
 *	Usage:						 
 *	Single file only				Example: ./MarkerInjector filepath0 <flags>
 *									Marks filepath0 and outputs result to dir of filepath0
 *	Input dir and output to dir		Example: ./MarkerInjector fileDir0 fileDir1
 *									Marks all files within fileDir0 recursively and outputs to fileDir1
 * 	Flags and examples:
 * 	-r 								Removes markers and related files from path/dirs.
 *  -a								Alternative format, uses experimental markers
 */

class CmdTasker{
private:
	struct config* cfg;	
	
	bool hasFlag(std::string flag){
		return std::find(cfg->inputFlags.begin(), cfg->inputFlags.end(), flag) != cfg->inputFlags.end();
	}
	
public:
	CmdTasker(struct config* cfg){
		this->cfg = cfg;
	}
	
	/**
	 * 	TODO: Convert to switch?
	 * 	Bepaalt args behalve argv[0], het programma zelf.
	 */
	void processCMDInput(int argc, char **argv){
		llvm::outs() << "Argsize: " <<cfg->inputArgs.size() << "\n";
		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];
			if (arg.rfind("--", 0) == 0) {	// Checks if the string starts with "--"
				cfg->inputFlags.push_back(arg);
			} 
			else if(arg.rfind("-", 0) == 0){// Checks if the string starts with "-"
				cfg->inputFlags.push_back(arg);
			} 
			else {
				llvm::outs() << "Arg "+ std::to_string(i) + ": " << arg << "\n";
				cfg->inputArgs.push_back(arg);
			}
		}
	}
	
	bool determineTaskQuantifier(int argc, char **argv){
		// TODO: Adjust/change insertion queueing and removal queueing.
//		if(cfg->taskmode == TaskMode::INSERT){
//			
//		} else {
//			
//		}
		switch(cfg->inputArgs.size()){
			case 1: // single file insertion 
				if(fs::is_regular_file(argv[1]) && fs::exists(argv[1])){
					cfg->inputFile = argv[1];
					cfg->inputDir =	(cfg->inputFile).has_parent_path() ? (cfg->inputFile).parent_path():".";
					cfg->outputDir = cfg->inputDir;
					cfg->taskquantifier = TASKQUANTIFIER::SINGLE;
					return true;
				}
				llvm::outs() << "Invalid or non-existing path \n";
				break;
			case 2: // (recursive) dir insertion with output dir
				if (fs::is_directory(argv[1]) && fs::exists(argv[1]) && fs::is_directory(argv[2]) && fs::exists(argv[2])){
					cfg->inputDir = argv[1];
					cfg->outputDir = argv[2];
					if(std::count(cfg->inputFlags.begin(),cfg->inputFlags.end(), "-l")){
						cfg->taskquantifier = TASKQUANTIFIER::LOCAL; // local dir procession
					} else {
						cfg->taskquantifier = TASKQUANTIFIER::RECURSIVE;
					}
					return true;
				} else if((fs::is_regular_file(argv[1]) && fs::exists(argv[1])) && fs::is_directory(argv[2]) && fs::exists(argv[2]) ){
					// TODO: ...
					llvm::outs() << "file dir input" << "\n";
					cfg->inputFile = argv[1];
					cfg->inputDir =	(cfg->inputFile).has_parent_path() ? (cfg->inputFile).parent_path():".";
					llvm::outs() << "file dir input" << "\n";

					cfg->outputDir = argv[2];
					cfg->taskquantifier = TASKQUANTIFIER::SINGLE;
					return true;
				}
				llvm::outs() << "Invalid or non-existing directory \n";
				break;
			default:
				break;
		}
		return false;
	}
	
	bool determineTaskMode(){
		// check mode
		cfg->taskmode = TASKMODE::INSERT;
		if (cfg->inputFlags.size() > 0) {
			if(hasFlag("-r")){				
				cfg->taskmode = TASKMODE::REMOVE;
			} else if (hasFlag("-ps")) {
				cfg->taskmode = TASKMODE::PRESCAN; 	// e.g. scan for occurences of markername
			} else if(hasFlag("-pp")){
				cfg->taskmode = TASKMODE::PREPROCESS;
			}
		}
		return true;
	}
	
	void determineFormat(config* cfg){
		if (hasFlag("-a")) {		// alternative format
			cfg->format = 1;
		} else {
			cfg->format = 0;
		}
	}
	
	// return bool for ok continuation?
	void processConfig(int argc, char **argv){
		processCMDInput(argc, argv);
		bool tmStatus = determineTaskMode();
		bool tqStatus = determineTaskQuantifier(argc, argv);
		determineFormat(cfg);

		cfg->isValid=tqStatus && tmStatus;
	}
};




