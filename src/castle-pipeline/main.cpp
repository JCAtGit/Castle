#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

/**
 * 	TODO:
 * 		Rekening houden met macros
 * 		Naar CLI11 ombouwen
 * 		Niet alleen markers toevoegen maar ook no-inline attribute toevoegen aan methoden/functies
 * 		Rework EVERYTHING SE-wise
 */

int main(int argc, char **argv) {
	CLI::App app{"Castle"};
	
//	string inputPath;
//	app.add_option("input", inputPath, "input path for instrumentation")
//		->required();
//	
//	string outputPath = inputPath; // overwrite location if no output is given.
//	auto option_outputPath = app.add_option("-o,--output", outputPath, "output path for results");
//	
//	bool onCopy;
//	app.add_flag("--on-copy",onCopy, "copy inputdir to outputdir and then perform enhancements on copy.")
//		->needs(option_outputPath);
//	
//	string scaffolderConfig = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/codebase-scaffolder/config/cs_config.json";
//	app.add_option("--scaffolder-config", scaffolderConfig, "Uses the specified config for the scaffolder");
//
//	string toolConfig = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/codebase-scaffolder/config/cg_config.json";
//	app.add_option("--tool-config", toolConfig, "Uses the specified config for the tools within scaffolder.");
//		
//	std::string mrkrFormatsPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/castle-pipeline/instrumentation-config/formats.json";
//	app.add_option("--mrkr_cfg_dir", mrkrFormatsPath, "Marker config used for heuristics");
	
	CLI11_PARSE(app, argc, argv);
//	CmdTasker cmdTasker(scaffolderConfig, toolConfig);
}

