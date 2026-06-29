#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include "clang/Basic/SourceManager.h"

namespace fs = std::filesystem;
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


bool is_subpath(const fs::path& base, const fs::path& sub) {
    // Make both absolute (uses current path for relative ones)
    auto base_abs = fs::absolute(base);
    auto sub_abs  = fs::absolute(sub);

    // Normalise (remove "." and "..", resolve symlinks if they exist)
    auto base_norm = fs::weakly_canonical(base_abs);
    auto sub_norm  = fs::weakly_canonical(sub_abs);

    // Compare component‑by‑component to check if base is a prefix of sub
    auto base_it = base_norm.begin();
    auto sub_it  = sub_norm.begin();

    while (base_it != base_norm.end() && sub_it != sub_norm.end()) {
        if (*base_it != *sub_it)
            return false;
        ++base_it;
        ++sub_it;
    }

    // If we exhausted all components of base, then base is a parent of sub
    return base_it == base_norm.end();
}

bool is_subpathOfDirs(set<string> &excludeDirsSet, const filesystem::path &entry){
	for(auto exclDir: excludeDirsSet){
		if(is_subpath(exclDir, entry)){
			return true;
		}
	}
	return false;
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