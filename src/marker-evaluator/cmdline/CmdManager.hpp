#include <CLI/CLI.hpp>
#include <filesystem>
#include "../config/config_ds.hpp"

class CmdManager{
	private:
		struct config* cfg;	
		// TODO: misschien samenvoegen met config
		
			
	public:
	
	
	CmdManager(struct config* cfg){
		CLI::App app{"MarkerBuilder"};
		CODE_ORIGIN_TYPE mode;
		app.add_option("-m,--mode", mode, "Operating mode: synthetic or organic")
			->transform(CLI::CheckedTransformer(mode_map, CLI::ignore_case))
			->default_val(CODE_ORIGIN_TYPE::SYNTHETIC);
			
		std::vector<std::filesystem::path> buildTargets;
		app.add_option("-bt,--build_targets", buildTargets, "Directories to build")
			->required(false);
			
		

	}
};
	