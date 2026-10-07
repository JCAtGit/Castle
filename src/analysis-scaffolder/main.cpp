#include <CLI/CLI.hpp>
#include <string>
#include "CmdTasker.hpp"

using namespace std;

// TODO: generalize by migrating config fetching from main to s_config?
int main(int argc, char **argv) {
	CLI::App app{"AnalysisScaffolder"};

	string scaffolderConfig = "/home/user/Desktop/CCastle/CCastleData/configs/AnalysisScaffolder/s_config.json";
	app.add_option("--scaffolder-config", scaffolderConfig, "<path-to-config>, uses the specified config for the scaffolder");

	string toolConfig = "/home/user/Desktop/CCastle/CCastleData/configs/AnalysisScaffolder/t_config.json";
	app.add_option("--tool-config", toolConfig, "<path-to-config>, uses the specified config for the tools within scaffolder.");
	// select oracle map, identify marker function in decompiled code
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr(scaffolderConfig, toolConfig);
	cmdTskr.startProcesses();
	cmdTskr.persistChanges();
}