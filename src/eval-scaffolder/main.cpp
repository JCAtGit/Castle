#include <CLI/CLI.hpp>
#include "../eval-scaffolder/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"EvalScaffolder"};

	string scaffolderConfig = "/home/user/Desktop/CCastle/CCastleData/configs/EvalScaffolder/es_config.json";
	app.add_option("--scaffolder-config", scaffolderConfig, "Uses the specified config for the scaffolder");

	string toolConfig = "/home/user/Desktop/CCastle/CCastleData/configs/EvalScaffolder/et_config.json";
	app.add_option("--tool-config", toolConfig, "Uses the specified config for the tools within scaffolder.");
	// select oracle map, identify marker function in decompiled code
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr(scaffolderConfig, toolConfig);
	cmdTskr.startEvaluationProcesses();
	cmdTskr.persistChanges();
}