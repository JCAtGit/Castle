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
	CLI::App app{"MarkerSurgeon"};
	
	std::string inputPath;
	app.add_option("input", inputPath, "input path for instrumentation")
		->required();
	
	std::string outputPath = inputPath; // overwrite location if no output is given.
	auto option_outputPath = app.add_option("-o,--output", outputPath, "output path for results");
	
	bool onCopy;
	app.add_flag("--on-copy",onCopy, "copy inputdir to outputdir and then perform enhancements on copy.")
		->needs(option_outputPath);
	
	int taskQuantifier;
	app.add_option("--task-quantifier", taskQuantifier, "Determines if files will be queued 0=recursively or 1=locally");
		
	std::string mrkrFormatsPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/castle-pipeline/instrumentation-config/formats.json";
	app.add_option("--mrkr_cfg_dir", mrkrFormatsPath, "Marker config used for heuristics");
	
	bool toggle = false;
	app.add_flag("-t, --toggle",toggle,  "Toggles marking and removal");
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr(inputPath, outputPath, mrkrFormatsPath, onCopy);
	if(toggle){
		// start instrumentation removal TODO
	} else {
		cmdTskr.insertInstrumentationEnhancement();
	}
}

