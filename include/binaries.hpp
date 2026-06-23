#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <array>

using namespace std;

void binaryMetadata(string binaryPath){
	// llvm-readelf --elf-output-style=JSON --pretty-print -h -p .comment binary
	string command = "llvm-readelf --elf-output-style=JSON --pretty-print -h -p .comment " + binaryPath;
	int status = system(command.c_str());
}



string execCommandAndCaptureOutput(const string& cmd, string& output) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    return output;
}