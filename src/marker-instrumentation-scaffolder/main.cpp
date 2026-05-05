
#include "cmdline/CmdTasker.hpp"
#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
	CLI::App app{"MarkerInstrumentationScaffolder"};
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr;
	cmdTskr.startInstrumentationProcesses();
	cmdTskr.persistChanges();
	return 0;
}
