#include <CLI/CLI.hpp>
#include "../config/config_ds.hpp"

class CmdManagerNext{
	private:
		struct config* cfg;	

		
	public:
	CmdManagerNext(config* cfg){
		CLI::App app{"MarkerSurgeon"};
		
		app.add_option("-f,--file", cfg->inputFile, "File to process")->required();

		// format, 
	}
};
	