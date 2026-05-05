#include "cmdline/CmdManager.hpp"
#include "config/config_ds.hpp"

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
	
	std::string outputPath;
	app.add_option("-o,--output", outputPath, "output path for results")
		->required();
	
//	std::string outputPath;
//		app.add_option("-o,--output", outputPath, "output path for results")
//			->required();
		
	CLI11_PARSE(app, argc, argv);
	
	// load config or use new one
//	auto cfg = std::make_shared<config>();
//	
//	CmdTasker cmdMgr(cfg.get());
//	TaskManager tskMgr(cfg.get());
//	cmdMgr.processConfig(argc, argv);
//	
//	if(cfg->isValid) {
//		tskMgr.start();
//	}
	return 0;
}

