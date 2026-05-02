#include <CLI/CLI.hpp>
#include "cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MarkerDecomScaffolder"};
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr;
	cmdTskr.startDecompilationProcesses();
	cmdTskr.persistChanges();
}
