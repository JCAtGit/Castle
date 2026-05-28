#include <CLI/CLI.hpp>
#include "../codebase-scaffolder/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"CodebaseScaffolder"};
	
	string scaffolderConfig = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/codebase-scaffolder/config/cs_config.json";
	app.add_option("--scaffolder-config", scaffolderConfig, "Uses the specified config for the scaffolder");

	string toolConfig = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/codebase-scaffolder/config/cg_config.json";
	app.add_option("--tool-config", toolConfig, "Uses the specified config for the tools within scaffolder.");
	
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr(scaffolderConfig, toolConfig);
	cmdTskr.startCodeGeneration();
	cmdTskr.persistChanges();
}
