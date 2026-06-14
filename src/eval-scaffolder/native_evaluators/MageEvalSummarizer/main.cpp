#include <CLI/CLI.hpp>

#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MageEvalSummarizer"};
	
	string inputPath;
	app.add_option("inputPath", inputPath, "input path to evaluate")
		->required();

	string outputPath;
	app.add_option("-o,--output", outputPath, "output path for results")
		->required();

	/**
	 * 	0 - local summarizer:
	 * 		high-level/generic stats per construct, e.g. basic recovery.
	 *
	 */
	int summaryMode = 0;
	app.add_option("-o,--output", outputPath, "output path for results");
	
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr(inputPath, outputPath);
}
