#pragma once
#include "llvm/Support/raw_ostream.h"

/**
 * 
 * Flags and examples:
 * 
 */
bool argParser(int argc, char **argv){
	// argv[0] is de programma zelf.
	if (argc < 3) {
		llvm::errs() 
			<< "Usage: " << argv[0] << " <input_file> <output_file>\n"
			<< "Type " << argv[0] << " --help or -h for the list of options.";
		return false;
	}
	
	return true;
}