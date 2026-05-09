
#include <CLI/CLI.hpp>
#include "../instrumentation-scaffolder/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MarkerInstrumentationScaffolder"};
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr;
	cmdTskr.startInstrumentationProcesses();
	cmdTskr.persistChanges();
	return 0;
}
