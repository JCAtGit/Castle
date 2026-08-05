#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"
#include <string>

/**
 *	Castle's native local set analysis/comparison tool.
 * 	Aggregates results from within a single set, producing a summary.
 */

int main(int argc, char **argv) {
	CLI::App app{"MageMarkerBinaryValidator"};
	
	string inputBuildDir; // assumes naming convention
	app.add_option("inputBuildDir", inputBuildDir, "Build dir to validate")
		->required();


	string outputDir;
	app.add_option("-o,--output", outputDir, "output dir for results")
		->required();

	string mrkrFormatsPath = "/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json";
	app.add_option("--marker_formats_path", mrkrFormatsPath, "Instrumentation config used for heuristics")
		->required();
		
	bool nameEncodedMarkers = false;
	app.add_flag("-n, --name_encoded_marker",nameEncodedMarkers,  "Evaluate constructs using names instead of indices.");
		
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
			
//	int analysisMode = 0;
//	app.add_option("-a,--analysisMode", analysisMode, "determines what analysis is performed");
	
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputBuildDir, outputDir, mrkrFormatsPath, nameEncodedMarkers, processMode, it_begin, it_end);
	cmdTskr.startProcesses();
}
