#include <CLI/CLI.hpp>

#include "../codebase-scaffolder/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"CodeScaffolder"};
	
	CLI11_PARSE(app, argc, argv);
	
	CmdTasker cmdTskr;
	cmdTskr.startCodeGeneration();
	cmdTskr.persistChanges();
}
