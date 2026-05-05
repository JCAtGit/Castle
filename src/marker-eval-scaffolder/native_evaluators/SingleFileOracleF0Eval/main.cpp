#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MarkerEvaluator0"};
	
	std::string inputPath;
	app.add_option("inputPath", inputPath, "input path to evaluate")
		->required();
	
	std::string oraclePath;
	app.add_option("oraclePath", oraclePath, "input path to use as oracle")
		->required();

		
	std::string outputPath;
	app.add_option("-o,--output", outputPath, "file to process")
		->required();
		
	std::string mrkrCfgDirDir;
	app.add_option("--mrkr_cfg_dir", mrkrCfgDirDir, "Marker details used for heuristics")
		->required();

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, oraclePath, outputPath, mrkrCfgDirDir);
	cmdTskr.startEvaluationProcesses();
}