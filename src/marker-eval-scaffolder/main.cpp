#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MarkerEvaluator"};

	// select oracle map, identify marker function in decompiled code
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr;
	cmdTskr.startEvaluationProcesses();
	cmdTskr.persistChanges();
}