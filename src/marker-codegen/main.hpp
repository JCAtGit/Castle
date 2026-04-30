
#include "cmdline/CmdManager.hpp"
#include "config/config_ds.hpp"

int main(int argc, char **argv) {
	auto cfg = std::make_shared<config>();
	CmdManager cmdMgr(argc, argv);
	
	// gather info about sys, repo, tools and other factors
	
	// create timestamp
	// callee sys info
	
	/**
	 * 	gather repo info
	 * 		- detect repo buildsystem (make, cmake)
	 *		- find repo build flags
	 */
	
	// gather tool info
	
	// modify build config if possible, then store build config and try build
	// een modus voor synthetic en er 1 voor organic, sla settings op in een json conf
	return 0;
}
