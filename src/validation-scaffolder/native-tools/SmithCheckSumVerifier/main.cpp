#include "binaries.hpp"
#include <CLI/CLI.hpp>
#include <string>

using namespace std;

/**
 *	Compares the checksum of 2 smith programs.
 */

int main(int argc, char **argv) {
	CLI::App app{"SmithCheckSumVerifier"};

	vector<filesystem::path> inputDirs;
	app.add_option("smiths", inputDirs, "<path-to-smith1-binary> <path-to-smith2-binary>")
		->required()
		->expected(-2);
	
	CLI11_PARSE(app, argc, argv);
	
	string smith0result, smith1result;
	
	execCommandAndCaptureOutput(inputDirs[0].string(), smith0result);
	execCommandAndCaptureOutput(inputDirs[1].string(), smith1result);
	
	cout << (smith0result == smith1result);
}