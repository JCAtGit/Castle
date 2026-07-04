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

/**
 * 	Voorbeeld:
 *
 */
bool substringOnlyAtEndOfBasename(const string &filename, const string &sub) {
	if (sub.empty())
		return false;

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


/**
 *	Checks if some path is a childpath of basepath
 *	@param	base 	parent path to match subpath against.
 *	@param	sub		potential childpath of base
 *	@return	true if sub is a childpath of base
 */
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

/**
 *	Checks if some entry is the subpath of any of the dirs in the excludeDirsSet.
 *	@param	excludeDirsSet	Exclusion set that invalidates an entry if any of the directories is a parent of the entry.
 *	@param	entry			Entry that is matched against the exclusion set.
 *	@return					True if entry is a subpath of any of the directories, false otherwise.
 */
bool is_subpathOfDirs(set<string> &excludeDirsSet, const filesystem::path &entry){
	for(auto exclDir: excludeDirsSet){
		if(is_subpath(exclDir, entry)){
			return true;
		}
	}
	return false;
}

/**
 *	Returns a string representing the relative traversal required to return to the base dir.
 *	Example 1:
 *		Input: 	"folder/subfolder/file.txt"
 *		Output: "../../"
 *	
 *	Example 2:
 *		Input: "a/b/c/d"
 *		Output: "../../../"
 *
 *	@param	countable	A path string
 *	@return	relative traversal to base dir string
 */
string getDescend2BaseDirString(string pathString){
	string descends = "";
	for(char ch:pathString){
		if(ch == '/'){
			descends = descends + "../";
		}
	}
	return descends;
}


/**
 *	Returns the contents of a file(path) as a string.
 *	@param 	filepath	absolute or relative filepath
 *	@return				contents of filepath as a string
 */
string file2string(string filepath) {
	string filestringdata;
	ifstream file(filepath);
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

/**
 *	Stores a string to a file(path)
 *	@param data string data to store
 *	@param path path where data will be stored
 */
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

/**
 *	Returns the leaf node of a path as a string, can be a dir or file.
 *	@param 	path	to extract the leaf string/path from
 *	@return			leaf string representing a dir or file
 */
string get_leaf_string(const string& path) {
	string p = path;
    // Edge case: Path consists entirely of slashes (e.g., "///" or "\\")
    if (path.find_first_not_of("/\\") == string::npos) {
        return "/"; // Return root
    }

    // Remove ALL trailing slashes (both / and \)
    while (!p.empty() && (p.back() == '/' || p.back() == '\\')) {
        p.pop_back();
    }

    // If the string is now empty (shouldn't happen due to the first check, but safe)
    if (p.empty()) {
        return "";
    }

    // Find the last occurrence of either slash type
    size_t pos = p.find_last_of("/\\");

    // If no slash found, the whole string is the leaf
    if (pos == string::npos) {
        return p;
    }

    // Return everything after the last slash
    return p.substr(pos + 1);
}