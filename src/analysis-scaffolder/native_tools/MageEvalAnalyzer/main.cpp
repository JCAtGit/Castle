#include <CLI/CLI.hpp>

#include "../MageEvalAnalyzer/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MageEvalSummarizer"};
	
	string inputDir;
	app.add_option("inputPath", inputDir, "input dir to evaluate")
		->required();

	string outputDir;
	app.add_option("-o,--output", outputDir, "output dir for results")
		->required();

	int processMode = 0;
	app.add_option("-o,--output", processMode, "set, element-all, element-it")
		->required();
	
	/**
	 *	Separated mode for easier testing later.
	 * 	0: 	perform every analysis
	 * 	1: 	high-level/generic stats per construct, e.g. basic recovery.
	 * 	 	general statistical analysis: 
	 *			bernouilli?, e.g.; p, variance, std deviation.
	 *	2:
	 *		
	 */
	int analysisMode = 0;
	app.add_option("-o,--output", analysisMode, "determines what analysis is performed");
	
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputDir, outputDir, processMode, analysisMode);
}
