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
	CLI::App app{"Mage"};
	
	string inputPath;
	app.add_option("input", inputPath, "input path for instrumentation")
		->required();
	
	string outputPath; // overwrite location if no output is given.
	auto option_outputPath = app.add_option("-o,--output", outputPath, "output path for results");
	
	bool onCopy = false;
	app.add_flag("--on-copy",onCopy, "copy inputdir to outputdir and then perform enhancements on copy.")
		->needs(option_outputPath);
	
	string mrkrFormatsPath = "/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json";
	app.add_option("--marker_formats_path", mrkrFormatsPath, "Marker format used for heuristics");
	
	bool toggle = false; // TODO: has been removed due to rework, reimplement someday
	app.add_flag("-t, --toggle",toggle,  "TODO: Toggles marking and removal");
	
	bool enchantFunction = false; // TODO: Is used automatically currently, implement as optional function 
	app.add_flag("-e, --enchant_function",enchantFunction,  "TODO: Adds (optnone, etc.) attributes to function decls.");

	bool nameEncodedMarker = false;
	app.add_flag("-n, --name_encoded_marker",nameEncodedMarker,  "Mark constructs using names instead of indices.");
	
	// TODO: Check parents for compoundStmt and integrate clangformat decision points. Should test first.
	bool clangFormatMode = true;	
	app.add_flag("-c, --clang_format_mode",clangFormatMode,  "TODO: Use clang-format and insert more liberally.");
	
	CLI11_PARSE(app, argc, argv);
	
	if(outputPath.empty()){
		outputPath = inputPath;
	}
	
	CmdTasker cmdTskr(inputPath, outputPath, mrkrFormatsPath, onCopy, nameEncodedMarker, clangFormatMode);
	if(toggle){
		// start instrumentation removal TODO
		cout << "TODO: Toggle \n";
	} else {
		cmdTskr.insertInstrumentationEnhancement();
	}
}

