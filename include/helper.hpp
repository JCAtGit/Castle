#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
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

	// Check that basename ends with 'sub' and that 'sub' does not appear
	// elsewhere
	if (basename.length() < sub.length())
		return false;
	if (basename.compare(basename.length() - sub.length(), sub.length(), sub) != 0)
		return false;

	// Ensure no earlier occurrence of 'sub' in basename
	size_t firstOccurrence = basename.find(sub);
	return firstOccurrence == basename.length() - sub.length();
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

void removeFirstLineIfStringMatches(clang::Rewriter &rewriter,
                                    clang::SourceManager &SM,
                                    const string &matchString) {
  clang::FileID mainFileID = SM.getMainFileID();
  clang::SourceLocation start = SM.getLocForStartOfFile(mainFileID);

  bool invalid = false;
  llvm::StringRef buffer = SM.getBufferData(mainFileID, &invalid);
  if (invalid) return;

  // Find the first newline character
  size_t newlinePos = buffer.find('\n');
  llvm::StringRef firstLine;
  size_t removeLength;  // length of characters to remove (including newline if present)

  if (newlinePos == llvm::StringRef::npos) {
    // No newline → the whole file is a single line
    firstLine = buffer;
    removeLength = buffer.size();   // remove everything
  } else {
    // Normal case: line ends with newline
    firstLine = buffer.substr(0, newlinePos);
    removeLength = newlinePos + 1;  // include the newline character
  }

  // Compare and remove only if the line matches exactly
  if (firstLine == matchString) {
    clang::SourceLocation end = start.getLocWithOffset(removeLength);
    clang::CharSourceRange lineRange = clang::CharSourceRange::getCharRange(start, end);
    rewriter.RemoveText(lineRange);
  }
}

void removeFirstLineIfContains(Rewriter &rewriter,
                               SourceManager &SM,
                               const std::string &substring) {
  FileID mainFileID = SM.getMainFileID();
  SourceLocation start = SM.getLocForStartOfFile(mainFileID);

  bool invalid = false;
  llvm::StringRef buffer = SM.getBufferData(mainFileID, &invalid);
  if (invalid) return;

  // Find the first newline character
  size_t newlinePos = buffer.find('\n');
  llvm::StringRef firstLine;
  size_t removeLength;

  if (newlinePos == llvm::StringRef::npos) {
    // No newline → whole file is a single line
    firstLine = buffer;
    removeLength = buffer.size();
  } else {
    firstLine = buffer.substr(0, newlinePos);
    removeLength = newlinePos + 1;   // include the newline
  }

  // Check if the first line contains the substring
  if (firstLine.contains(substring)) {
    SourceLocation end = start.getLocWithOffset(removeLength);
    CharSourceRange lineRange = CharSourceRange::getCharRange(start, end);
    rewriter.RemoveText(lineRange);
  }
}
