#include <filesystem>
#include <iostream>

using namespace std;

void preparePaths(string inputPath, string  outputPath){
	std::filesystem::remove_all(outputPath);
	std::filesystem::copy(inputPath, outputPath);
}