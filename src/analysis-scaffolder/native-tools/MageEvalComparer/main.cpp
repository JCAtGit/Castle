#include <CLI/CLI.hpp>

#include "../../native-tools/MageEvalComparer/cmdline/CmdTasker.hpp"
/**
 *	Castle's native cross set comparison tool.
 *  Summarizes over multiple sets and performs statistics over the unified numbers.
 */
int main(int argc, char **argv) {
	CLI::App app{"MageEvalComparer"};
	
	vector<filesystem::path> inputDirs;
	app.add_option("inputDirs", inputDirs, "Input MageEvalAnalysis-phase directories to aggregate results from.")
		->required()
		->expected(-2);
		
	filesystem::path outputDir;
	app.add_option("-o,--output", outputDir, "Output dir for aggregated results")
		->required();
	
	bool recombine_only = false;
	app.add_flag("--recombine-only", recombine_only, "Does not attempt to rebuild the summary using the build data.");
	
	filesystem::path buildScaffolderDir = "/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/";
	app.add_option("-b,--buildScaffolderDir", buildScaffolderDir, "Required input buildScaffolderDir for binary data extraction.");

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputDirs, outputDir, buildScaffolderDir, recombine_only);
	cmdTskr.startProcesses();
}
