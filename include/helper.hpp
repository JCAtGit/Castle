#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace std;
using namespace clang;

// General miscellaneous implementations are gathered here for now.

/**
 * 	Voorbeeld:
 *
 */
bool substringOnlyAtEndOfBasename(const string &filename,
								  const string &sub) {
	if (sub.empty())
		return false; // or handle as you wish

	// Find the last dot (extension separator)
	size_t dotPos = filename.rfind('.');
	string basename =
		(dotPos == string::npos) ? filename : filename.substr(0, dotPos);

	// Check that basename ends with 'sub' and that 'sub' does not appear elsewhere
	if (basename.length() < sub.length())
		return false;
	if (basename.compare(basename.length() - sub.length(), sub.length(), sub) != 0)
		return false;

	// Ensure no earlier occurrence of 'sub' in basename
	size_t firstOccurrence = basename.find(sub);
	return firstOccurrence == basename.length() - sub.length();
}

bool is_subpath(const filesystem::path& base, const filesystem::path& sub) {
    auto base_norm = filesystem::weakly_canonical(base);
    auto sub_norm = filesystem::weakly_canonical(sub);
    auto rel = sub_norm.lexically_relative(base_norm);
    return !rel.empty() && rel.native()[0] != '.' && rel.native()[0] != '/' || rel == ".";
}

/**
 *	Voorbeeld:
 * 	
 *
 */
string rel_outputPath(string dirIn, string pathIn) {
	filesystem::path rel_path = filesystem::relative(pathIn, dirIn);
	return rel_path;
}


string getDescend2BaseDirString(string countable){
	string descends = "";
	for(char ch:countable){
		if(ch == '/'){
			descends = descends + "../";
		}
	}
	return descends;
}


// Can use absolute and relative filepath
string file2string(string filepath) {
	string filestringdata;
	ifstream file(filepath);

	// confirm file opening
	if (!file.is_open()) {
		cerr << "Failed to open file: " << filepath << endl;
		return ("file2string error for: " + filepath + "\n");
	}

	// Read the file line by line into a string
	string line;
	while (getline(file, line)) {
		filestringdata += line + "\n";
	}

	file.close();
	return filestringdata;
}

void string2file(string data,  string path){
	// Open the file; if it exists, it will be overwritten.
    error_code EC;
    llvm::raw_fd_ostream out(
		path, EC, llvm::sys::fs::OF_None
	);

    // Check for errors during file opening.
    if (EC) {
        llvm::errs() << "Error opening file: " << EC.message() << "\n";
    }

    // Write the string to the file.
    out << data;

    // Optionally flush (the destructor will close the file automatically).
    out.flush();
	llvm::outs() << "Definition file written. \n";
}

std::string get_leaf_string(const std::string& path) {
    // Edge case: Path consists entirely of slashes (e.g., "///" or "\\")
    if (path.find_first_not_of("/\\") == std::string::npos) {
        return "/"; // Return root
    }

    std::string p = path;

    // 1. Remove ALL trailing slashes (both / and \)
    while (!p.empty() && (p.back() == '/' || p.back() == '\\')) {
        p.pop_back();
    }

    // 2. If the string is now empty (shouldn't happen due to the first check, but safe)
    if (p.empty()) {
        return "";
    }

    // 3. Find the last occurrence of either slash type
    size_t pos = p.find_last_of("/\\");

    // 4. If no slash found, the whole string is the leaf
    if (pos == std::string::npos) {
        return p;
    }

    // 5. Return everything after the last slash
    return p.substr(pos + 1);
}