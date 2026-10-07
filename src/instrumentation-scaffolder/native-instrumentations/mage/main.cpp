#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

/**
 *	Castle's native instrumentation tool.
 *	This tool utilizes attributes, preprocessor directives and "marker" functions to map oracle and decompilation sources.
 *	Mage implements two kinds of markers formats:
 * 		1. Standard, 		a function which has an integer argument representing it's identifier.
 *		2. Name encoded, 	a function which has it's identifying integer embedded in its name declaration.
 *			This is a failsafe mechanism for decompilation that counteracts lost arguments.
 *	
 * 	TODO: 
 *		- Addition of dangling child check, only useful if decompilers get more accurate.
 *		Include deeper inspection of dangling stmts, which is useful when not using the formatter.
 *		The inspection should be relatively easy, since without a compound child stmt, we only have a single child to check for.
 */

int main(int argc, char **argv) {
	CLI::App app{"Mage"};
	
	string inputPath;
	app.add_option("input", inputPath, "input path for instrumentation")
		->required();

	string outputPath; // overwrites location if no output is given.
	auto option_outputPath = app.add_option("-o,--output", outputPath, "output path for results, requires on-copy to preserve build processes");
	
	bool onCopy = false;
	auto option_onCopy = app.add_flag("--on-copy",onCopy, "copy inputdir to outputdir and then perform enhancements on copy.")
		->needs(option_outputPath);
	
	option_outputPath->needs(option_onCopy); // required when outputting to ensure build process preservation.
	
	string mrkrFormatsPath = "/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json";
	app.add_option("--marker_formats_path", mrkrFormatsPath, "Marker details used for heuristics.");

	string metadataResultsPath = "/home/user/Desktop/CCastle/CCastleData/InstrumentationScaffolder/metadata/";
	app.add_option("--metadata_results_path", metadataResultsPath, "Path for Storing metadata from the instrumentation process.");
	
	// Sometimes decompiler, like Ghidra, fail to recover the index of markers but do recover the symbol information (name), this option serves as an robust alternative.
	bool nameEncodedMarker = false;
	app.add_flag("-n, --name_encoded_marker",nameEncodedMarker,  "Mark constructs using names instead of indices.");
	
	bool clangFormatMode = false;	
	app.add_flag("-c, --clang_format_mode",clangFormatMode,  "Use clang-format and potentially allow more liberal insertions, warning: clang-format can cause errors!");
	
	vector<string> excludeFiles;
	auto option_excludeFiles = app.add_option("--exclude_files", excludeFiles, "Files to exclude from processing");

	vector<string> excludeDirs;
	auto option_excludeDirs = app.add_option("--exclude_dirs", excludeDirs, "Directories to exclude from processing");

	CLI11_PARSE(app, argc, argv);

	set<string> excludeFilesSet(excludeFiles.begin(), excludeFiles.end());
	set<string> excludeDirsSet(excludeDirs.begin(), excludeDirs.end());

	CmdTasker cmdTskr(
		inputPath, 
		outputPath, 
		mrkrFormatsPath, 
		metadataResultsPath,
		onCopy, 
		nameEncodedMarker, 
		clangFormatMode, 
		excludeFilesSet,
		excludeDirsSet
	);
		
	cmdTskr.insertInstrumentationEnhancement();
}

