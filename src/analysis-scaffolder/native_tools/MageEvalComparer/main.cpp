#include <CLI/CLI.hpp>

#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MageEvalAnalyzer"};
	
	vector<filesystem::path> inputDirs;
	app.add_option("inputDirs", inputDirs, "Input directories to aggregate results from.")
		->required()
		->expected(-2);
		
	filesystem::path outputDir;
	app.add_option("-o,--output", outputDir, "Output dir for aggregated results")
		->required();

	string centralizeResultsDir = outputDir;
	auto option_centralizeResultsPath = app.add_option("--centralize_results_dir", centralizeResultsDir, "Result tally path, use with --centralize_results.");	

	bool centralizeResults = false;
	app.add_option("--centralize_results", centralizeResults, "Tally all the results in one path, results that are included will be overridden and recalculated.")
		->needs(option_centralizeResultsPath);
	
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputDirs, outputDir, centralizeResultsDir, centralizeResults);
	cmdTskr.startProcesses();
}
