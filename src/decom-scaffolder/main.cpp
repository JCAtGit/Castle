#include <CLI/CLI.hpp>

//#include "../decom-scaffolder/cmdline/CmdTasker.hpp"
#include "CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"DecomScaffolder"};
	
	string scaffolderConfig = "/home/user/Desktop/CCastle/CCastleData/configs/DecomScaffolder/ds_config.json";
	app.add_option("--scaffolder-config", scaffolderConfig, "Uses the specified config for the scaffolder");

	string toolConfig = "/home/user/Desktop/CCastle/CCastleData/configs/DecomScaffolder/dt_config.json";
	app.add_option("--tool-config", toolConfig, "Uses the specified config for the tools within scaffolder.");
	
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr(scaffolderConfig, toolConfig);
	cmdTskr.startProcesses();
	cmdTskr.persistChanges();
}
