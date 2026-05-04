#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MarkerEvaluator0"};
	
	std::string inputPath;
	app.add_option("input", inputPath, "input file to evaluate")
		->required();   // the user must provide it as a standalone argument
	
	std::string outputPath;
	app.add_option("-o,--output", outputPath, "file to process")
		->required();
	
	std::string mode;
	app.add_option("-m,--mode", mode, "oracle mode")
		->required();
		
	std::string mrkrCfgPath;
	app.add_option("--mrkr_cfg", mrkrCfgPath, "Marker details used for heuristics")
		->required();

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, outputPath, mode, mrkrCfgPath);
	cmdTskr.startEvaluationProcesses();
}