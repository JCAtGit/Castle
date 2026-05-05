#pragma once
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

// TODO: Moet dit allemaal misschien naar .../constants/ ?

enum class TASKQUANTIFIER {
	UNDEFINED,
	SINGLE,		// Input 1 path:									Verwerk 1 bestand
	LOCAL,		// Input 1 input dir en output dir met de -l flag: 	Verwerk alleen de lokale bestanden in de dir zelf
	RECURSIVE	// Input 1 input dir en output dir:					Verwerk alle bestanden in de dir en subdirs.
};

enum class TASKMODE {
	UNDEFINED,	
	PRESCAN, 	// -ps:		Kan zaken omvatten als het controleren of de markernaam al in gebruik is binnen de code
	PREPROCESS,	// -pp:		Clang formatter, macro-expander
	INSERT,		// noflag: 	Invoegen van markers
	REMOVE,		// -r: 		Verwijderen van markers
};
/**
 * Baseconfig is used if no config is loaded within the program.
 * De config struct is de datastructuur alles inlaadt vanuit de cmdline en config.json.
 */
struct config {
	bool isValid = false;
	fs::path inputFile = "";
 	fs::path inputDir = "";
 	fs::path outputDir = "";
	std::vector<std::string> inputArgs;
	std::vector<std::string> inputFlags;
	std::vector<std::string> ToolArgs = {
		"-xc", 							// Treat the input as C code
		"-std=c11", 					// Use a specific C standard
		"-nostdinc",					// Don't search for system headers
//      "-D__STDC__=1",						//
		"-Wno-all",						// Suppress all warnings, W-prefix stands for warning.
	};

	// construct processor
	int format = 0;
	
	// Task section, aparte struct?
	TASKQUANTIFIER taskquantifier = TASKQUANTIFIER::UNDEFINED;
	TASKMODE taskmode = TASKMODE::UNDEFINED;
	
	// Marker section, aparte struct?
	std::string srcExt = 					// Appends srcExt to stem of filename after processing
//		"_marked";
		"";
	std::string markerName = "codemarker_alpha"; // do not add spaces before or after.
	std::string markerKeyword = "extern ";
	std::string markerFunctionTypeModifier = "static inline ";
	std::string markerFunctionType = "void ";
	std::string markerDefinitionFileName = "MarkerDefinition";
	std::string markerParamSignature = "(int markerID)";
	std::string markerDefinition = "{volatile int dummy = markerID;}";
	
	std::string markerFunctionSignatureString =  markerFunctionType + markerName + markerParamSignature + ";";
	std::string markerDefinitionString = markerFunctionType + markerName + markerParamSignature + markerDefinition;
	
	/** Unused */
	std::vector<fs::path> excludeDirs; // /build/ is er 1 voor cmake
	std::vector<fs::path> exlcudeFiles;
	
	std::vector<std::string> extensions2mark;
};

// sub configs?
struct InputConfig {};
struct TaskConfig {};
struct MarkerFormattingConfig {};

 
 