#include <CLI/CLI.hpp>

#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MageEval"};
	
	string inputPath;
	app.add_option("inputPath", inputPath, "input path to evaluate")
		->required();
	
	string oracleDir;
	app.add_option("oraclePath", oracleDir, "input dir to use as oracle, uses input filename in dir")
		->required();

	string outputPath;
	app.add_option("-o,--output", outputPath, "output path for results")
		->required();
		
	string mrkrFormatsPath = "/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json";
	app.add_option("--marker_formats_path", mrkrFormatsPath, "Instrumentation config used for heuristics")
		->required();
		
	bool nameEncodedMarkers = false;  // TODO
	app.add_flag("-n, --name_encoded_marker",nameEncodedMarkers,  "TODO: Evaluate constructs using names instead of indices.");
		
	// TODO: integrate option into implementation
	string astMode = "-xc"; 
	app.add_option("--astMode", astMode, "TODO: AST mode used for heuristics ('-xc' or '-xc++')")
	    ->check(CLI::IsMember({"-xc", "-xc++"}));

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, oracleDir, outputPath, mrkrFormatsPath, nameEncodedMarkers);
	cmdTskr.startEvaluationProcesses();
}
