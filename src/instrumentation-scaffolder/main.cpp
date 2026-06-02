
#include <CLI/CLI.hpp>
#include "../instrumentation-scaffolder/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"InstrumentationScaffolder"};
	
	string scaffolderConfig = "/home/user/Desktop/CCastle/CCastleData/configs/InstrumentationScaffolder/is_config.json";
	app.add_option("--scaffolder-config", scaffolderConfig, "Uses the specified config for the scaffolder");

	string toolConfig = "/home/user/Desktop/CCastle/CCastleData/configs/InstrumentationScaffolder/it_config.json";
	app.add_option("--tool-config", toolConfig, "Uses the specified config for the tools within scaffolder.");
	
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(scaffolderConfig, toolConfig);
	cmdTskr.startInstrumentationProcesses();
	cmdTskr.persistChanges();
	return 0;
}
