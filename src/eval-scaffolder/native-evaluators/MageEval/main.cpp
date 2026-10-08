#include <CLI/CLI.hpp>

#include "../../native-evaluators/MageEval/cmdline/CmdTasker.hpp"

/**
 *	Castle's native evaluation tool.
 * 	Gathers findings from oracle, decom sets and performs rudimentary comparisons.
 */
int main(int argc, char **argv) {
	CLI::App app{"MageEval"};
	
	string inputPath;
	app.add_option("inputPath", inputPath, "input path to evaluate")
		->required();
	
	string oracleDir;
	app.add_option("oracleDir", oracleDir, "input dir to use as oracle, uses related filenames in dir")
		->required();

	string outputPath;
	app.add_option("-o,--output", outputPath, "output path for results")
		->required();
		
	string mrkrFormatsPath = "/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json";
	app.add_option("--marker_formats_path", mrkrFormatsPath, "Instrumentation config used for heuristics")
		->required();
		
	bool nameEncodedMarkers = false;
	app.add_flag("-n, --name_encoded_marker",nameEncodedMarkers,  "Evaluate constructs using names instead of indices.");

	bool chiralMatching = false;
	app.add_flag("-c, --chiral_matching",chiralMatching,  "Matches different oracle-decom path type combinations.");

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, oracleDir, outputPath, mrkrFormatsPath, nameEncodedMarkers, chiralMatching);
	
	cmdTskr.startProcesses();
}
