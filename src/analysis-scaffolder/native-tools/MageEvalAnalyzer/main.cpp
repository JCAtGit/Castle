#include <CLI/CLI.hpp>

#include "../../native-tools/MageEvalAnalyzer/cmdline/CmdTasker.hpp"

/**
 *	Castle's native local set analysis/comparison tool.
 * 	Aggregates results from within a single set, producing a summary.
 */

int main(int argc, char **argv) {
	CLI::App app{"MageEvalAnalyzer"};
	
	string inputDir;
	app.add_option("inputPath", inputDir, "input dir to evaluate")
		->required();

	string outputDir;
	app.add_option("-o,--output", outputDir, "output dir for results")
		->required();

	// element-it assumes filename has starting incrementing integer or ending incrementing integer before the file extension.
	int processMode = 0;
	auto option_processMode = app.add_option(
		"-p, --processMode", 
		processMode, 
		"0 = set, process all files as part of a single set;  \
							 1 = element-all, process all files as their own entity; \
							 2 = element-it, process all it-elemented files as their own entity."
	)
		->required();
	
	int it_begin = 0;
	app.add_option("--it_begin", it_begin, "Sets beginning it-counter for index-named files")
		->needs(option_processMode);
			
	int it_end = 0;
	app.add_option("--it_end", it_end, "Sets end it-counter for index-named files")
		->needs(option_processMode);
			
	string metadataResultsPath = "/home/user/Desktop/CCastle/CCastleData/AnalysisScaffolder/metadata/";
	app.add_option("--metadata_results_path", metadataResultsPath, "TODO: Path for Storing metadata from the analysis process.");

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputDir, outputDir, processMode, it_begin, it_end, metadataResultsPath);
	cmdTskr.startProcesses();
}
