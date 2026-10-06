#include <CLI/CLI.hpp>
#include <string>
#include "CmdTasker.hpp"

using namespace std;

int main(int argc, char **argv) {
	CLI::App app{"ValidationScaffolder"};

	string scaffolderConfig = "/home/user/Desktop/CCastle/CCastleData/configs/ValidationScaffolder/s_config.json";
	app.add_option("--scaffolder-config", scaffolderConfig, "Uses the specified config for the scaffolder");

	string toolConfig = "/home/user/Desktop/CCastle/CCastleData/configs/ValidationScaffolder/t_config.json";
	app.add_option("--tool-config", toolConfig, "Uses the specified config for the tools within scaffolder.");
	// select oracle map, identify marker function in decompiled code
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr(scaffolderConfig, toolConfig);
	cmdTskr.startProcesses();
	cmdTskr.persistChanges();
}