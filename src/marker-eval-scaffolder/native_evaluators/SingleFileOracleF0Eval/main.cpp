#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"SingleFileOracleF0Eval"};
	
	std::string inputPath;
	app.add_option("inputPath", inputPath, "input path to evaluate")
		->required();
	
	std::string oraclePath;
	app.add_option("oraclePath", oraclePath, "input path to use as oracle")
		->required();

	std::string outputPath;
	app.add_option("-o,--output", outputPath, "output path for results")
		->required();
		
	std::string mrkrCfgDir;
	app.add_option("--mrkr_cfg_dir", mrkrCfgDir, "Marker config used for heuristics")
		->required();

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, oraclePath, outputPath, mrkrCfgDir);
	cmdTskr.startEvaluationProcesses();
}