#include <iostream>
#include <filesystem>
#include "clang/Tooling/Tooling.h"

#include "../marker-injector/filereader/filereader.h"

namespace fs = std::filesystem;

using namespace clang;

string rel_outputPath(string dirIn, string pathIn){
	fs::path rel_path = fs::relative(pathIn, dirIn);
	return rel_path;	
}

void ensureDirExists(fs::path src_path, fs::path dst_path){
	if (fs::is_directory(src_path)) {
        // Create corresponding directory in destination.
        fs::create_directories(dst_path);
        std::cout << "Created directory: " << dst_path << '\n';
    } else if (fs::is_regular_file(src_path)) {
        // Ensure parent directory exists.
        fs::create_directories(dst_path.parent_path());
		std::cout << "Created directory: " << dst_path.parent_path() << '\n';
    }
	// Symlinks, sockets, etc. are ignored.
}

void traverse(const fs::path& dirIn, const fs::path& dirOut) {
	std::vector <std::string> ToolArgs = {
			"-xc", 
			"-std=c11", 
			"-nostdinc",
	//      "-D__STDC__=1",
			"-Wno-all"
	};
	for (const auto& entry : fs::recursive_directory_iterator(dirIn)) {

//		std::cout << entry.path() << '\n';
        // If you need to process only regular files:
        // if (fs::is_regular_file(entry.status())) { ... }
		
		if (entry.path().extension() == ".c") {
			std::cout << "entry.path(): "<< entry.path() << '\n';

			auto unit = tooling::buildASTFromCodeWithArgs(file2string(entry.path().c_str()), ToolArgs, entry.path().string());
		    // process
			if(unit){
				std::cout << "---	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-\n";
				std::cout << "unit->getASTFileName(): " << unit->getASTFileName().str() << '\n';
				std::cout << "unit->getMainFileName(): " << unit->getMainFileName().str() << '\n';
				std::cout << "unit->getOriginalSourceFileName(): " << unit->getOriginalSourceFileName().str() << '\n';
				std::cout << "rel_outputPath: " << rel_outputPath(dirIn, unit->getMainFileName().str()) << '\n' ;
				std::cout << "complete outputhPath: " << dirOut / rel_outputPath(dirIn, unit->getMainFileName().str()) << '\n';
				std::cout << "---	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-\n";
			} else {
				std::cout << "ptr null" << "\n\n";
			}
		}
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input-directory> <output-directory>\n";
        return 1;
    }
    traverse(argv[1], argv[2]);
    return 0;
}