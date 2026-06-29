#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"
/**
 *	Castle's native cross set analysis/comparison tool.
 *  Summarizes over multiple sets and performs statistics over the unified numbers.
 */
int main(int argc, char **argv) {
	CLI::App app{"MageEvalComparer"};
	
	vector<filesystem::path> inputDirs;
	app.add_option("inputDirs", inputDirs, "Input directories to aggregate results from.")
		->required()
		->expected(-2);
		
	filesystem::path outputDir;
	app.add_option("-o,--output", outputDir, "Output dir for aggregated results")
		->required();
	
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputDirs, outputDir);
	cmdTskr.startProcesses();
}
