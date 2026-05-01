#include "cmdline/CmdManager.hpp"
#include "config/config_ds.hpp"
#include "task-manager/TaskManager.hpp"

//using namespace clang;

/**
 * 	TODO:
 * 		Rekening houden met macros
 * 		Naar CLI11 ombouwen
 * 		Niet alleen markers toevoegen maar ook inline attribute toevoegen aan methoden/functies
 * 		Rework EVERYTHING SE-wise
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

