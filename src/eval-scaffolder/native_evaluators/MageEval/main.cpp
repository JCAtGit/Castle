#include <CLI/CLI.hpp>

#include "../MageEval/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"SingleFileOracleF0Eval"};
	
	std::string inputPath;
	app.add_option("inputPath", inputPath, "input path to evaluate")
		->required();
	
	std::string oracleDir;
	app.add_option("oraclePath", oracleDir, "input dir to use as oracle, uses input filename in dir")
		->required();

	std::string outputPath;
	app.add_option("-o,--output", outputPath, "output path for results")
		->required();
		
	std::string mrkrFormatsPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/castle-pipeline/instrumentation-config/formats.json";
	app.add_option("--mrkrFormatsPath", mrkrFormatsPath, "Instrumentation config used for heuristics")
		->required();

	// TODO: integrate option into implementation
	std::string astMode = "-xc"; 
	app.add_option("--astMode", astMode, "AST mode used for heuristics (must be '-xc' or '-xc++')")
	    ->check(CLI::IsMember({"-xc", "-xc++"}));

	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, oracleDir, outputPath, mrkrFormatsPath);
	cmdTskr.startEvaluationProcesses();
}
