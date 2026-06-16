#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

/**
 * 	TODO:
 * 		Rekening houden met macros
 * 		Naar CLI11 ombouwen
 * 		Niet alleen markers toevoegen maar ook no-inline attribute toevoegen aan methoden/functies
 * 		Rework EVERYTHING SE-wise
 */

 struct { // TODO: part of rework
 		set<string> excludeFilesSet;
 		set<string> excludeDirsSet;
 		set<string> exclusiveFilesSet;
 		set<string> exclusiveDirsSetDirsSet;
 		bool onCopy = false;
		string mrkrFormatsPath;
 		bool nameEncodedMarker = false;
 		bool clangFormatMode = false;
 } cliVars;
 	
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
	
	bool clangFormatMode = false;	
	app.add_flag("-c, --clang_format_mode",clangFormatMode,  "Use clang-format and insert more liberally.");
	
	vector<string> excludeFiles;
	auto option_excludeFiles = app.add_option("--exclude_files", excludeFiles, "Files to exclude from processing");
	
	vector<string> excludeDirs;
	auto option_excludeDirs = app.add_option("--exclude_dirs", excludeFiles, "TODO: Directories to exclude from processing");

	vector<string> exclusiveFiles;
	auto option_exclusiveFiles = app.add_option("--exclusive_files", exclusiveFiles, "TODO: Dir's to exclusively visit for processing enqueuement ");
	
	vector<string> exclusiveDirs;
	auto option_exclusiveDirs = app.add_option("--exclusive_dir", exclusiveDirs, "TODO: Dir's to exclusively visit for processing enqueuement ");

	option_excludeFiles->excludes(option_exclusiveFiles, option_exclusiveDirs);
	option_excludeDirs->excludes(option_exclusiveFiles, option_exclusiveDirs);
	option_exclusiveFiles->excludes(option_excludeFiles, option_excludeDirs);
	option_exclusiveDirs->excludes(option_excludeFiles, option_excludeDirs);

	CLI11_PARSE(app, argc, argv);
	set<string> excludeFilesSet(excludeFiles.begin(), excludeFiles.end());
	set<string> excludeDirsSet(excludeDirs.begin(), excludeDirs.end());
	set<string> exclusiveFilesSet(exclusiveFiles.begin(), exclusiveFiles.end());
	set<string> exclusiveDirsSetDirsSet(exclusiveDirs.begin(), excludeDirs.end());

	CmdTasker cmdTskr(
		inputPath, 
		outputPath, 
		mrkrFormatsPath, 
		onCopy, 
		nameEncodedMarker, 
		clangFormatMode, 
		excludeFilesSet,
		excludeDirsSet
	);
	
	if(toggle){
		// start instrumentation removal TODO, or implement as separate tool?
		cout << "TODO: Toggle \n";
	} else {
		cmdTskr.insertInstrumentationEnhancement();
	}
}

