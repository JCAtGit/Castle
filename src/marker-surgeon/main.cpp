#include "cmdline/CmdManager.hpp"
#include "config/config_ds.hpp"
#include "task-manager/TaskManager.hpp"

//using namespace clang;

/**
 * Is part of pipeline composition:
 * 1. Marker-inserter: Main for marker inserter. TODO: MACROS!!!!!!!!!!!!!!!
 *
 * Wat zijn de mogelijke args? We kiezen voor de volgende: TODO Naar CLI11 ombouwen
 * 1. filePath.ext 	fileDir(optional)			- uses current dir to output marked file, include flag for srcname extension?
 * 2. fileDir		fileDir(required)			- ...
 * 
 * Niet alleen markers toevoegen maar ook inline attribute toevoegen aan methoden/functies
 */

int main(int argc, char **argv) {
	// load config or use new one
	auto cfg = std::make_shared<config>();
	
	CmdTasker cmdMgr(cfg.get());
	TaskManager tskMgr(cfg.get());
	cmdMgr.processConfig(argc, argv);
	
	if(cfg->isValid) {
		tskMgr.start();
	}
	return 0;
}

