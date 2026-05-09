
#include <CLI/CLI.hpp>
#include "../build-scaffolder/cmdline/CmdTasker.hpp"

int main(int argc, char **argv) {
	CLI::App app{"MarkerBuildScaffolder"};
	CLI11_PARSE(app, argc, argv);

	CmdTasker cmdTskr;
	cmdTskr.startBuildProcesses();
	cmdTskr.persistChanges();
	return 0;
}
