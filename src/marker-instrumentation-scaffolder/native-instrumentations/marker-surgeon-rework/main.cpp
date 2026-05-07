#include "cmdline/CmdTasker.hpp"
#include "config/config_ds.hpp"
#include "cmdline/Preparer.hpp"

/**
 * 	TODO:
 * 		Rekening houden met macros
 * 		Naar CLI11 ombouwen
 * 		Niet alleen markers toevoegen maar ook inline attribute toevoegen aan methoden/functies
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
	app.add_flag("--on-copy",onCopy, "output path for results")
		->needs(option_outputPath)
		->required();
	
	std::string mrkrFormatsPath;
	app.add_option("--mrkr_cfg_dir", mrkrFormatsPath, "Marker config used for heuristics")
		->required();
		
	CLI11_PARSE(app, argc, argv);
	
	if(onCopy){
		preparePaths(inputPath, outputPath);
	}
	CmdTasker cmdTskr(inputPath,outputPath, mrkrFormatsPath);
//	TaskManager tskMgr(cfg.get());
//	cmdMgr.processConfig(argc, argv);
//	
//	if(cfg->isValid) {
//		tskMgr.start();
//	}
	return 0;
}

