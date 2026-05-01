
#include "cmdline/CmdManager.hpp"
#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
	CLI::App app{"MarkerCodeScaffolder"};
	CLI11_PARSE(app, argc, argv);

	CmdManager cmdMgr;
	return 0;
}
