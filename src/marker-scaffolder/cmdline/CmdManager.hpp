#include <CLI/CLI.hpp>
#include <filesystem>
#include "../config/config_ds.hpp"
#include "nlohmann/json.hpp"

using namespace nlohmann;

class CmdManager{
	private:
		struct config* cfg;	
		// TODO: misschien samenvoegen met config
		
		bool use_config;
		BUILD_MODE build_mode;
		std::vector<std::filesystem::path> buildTargets;
		CODE_ORIGIN_TYPE cot;
		unsigned int it_cycles;		

	public:
	
	
	CmdManager(int argc, char **argv){
		CLI::App app{"MarkerBuilder"};

		// use config or cmd options
		app.add_option("-uc,--use_config", use_config, "Use config to run tool")
			->default_val(true);
			
		// initial selectors
		app.add_option("-tm,--tool_mode", build_mode, "Select a tool mode: generate or build")
			->transform(CLI::CheckedTransformer(mode_map, CLI::ignore_case))
			->default_val(CODE_ORIGIN_TYPE::SYNTHETIC);
			
		// code build conditional options
		app.add_option("-bt,--build_targets", buildTargets, "Directories to build")
			->required(false);
		
		// code compilation options
		app.add_option("-cot,--code_origin_type", cot, "Code origin type: synthetic or organic");
			
		// code gen conditional options
		app.add_option("-ic,--it_cycles", it_cycles, "Amount of gen cycles to actuate");

		CLI11_PARSE(app, argc, argv);
		
		if(use_config){
			loadConfig();
			switch (build_mode) {
				case BUILD_MODE::GENERATE:
				
				break;
				case BUILD_MODE::BUILD:
				
				break;
				case BUILD_MODE::GENERATE_AND_BUILD:
				break;
			}

			// exec de eerste cmd 
			//		status = std::system(command.c_str());

		} else {
			
		}
	}
	
	void loadConfig(){
		// 1. Open the file, looks for config in local dir.
	   std::ifstream file("config.json");
	   if (!file.is_open()) {
	       std::cerr << "Error: Could not open file 'config.json'" << std::endl;
	       return;
	   }

	   // 2. Parse the file stream
	   json data;
	   try {
	       data = json::parse(file);
	   } catch (const json::parse_error& e) {
	       std::cerr << "Parse error: " << e.what() << std::endl;
	       return;
	   }

	   // 3. Access the data (example: reading values from a config file)
	   std::string name = data.value("name", "DefaultName");
	   int version = data.value("version", 0);
	   bool debug = data.value("debug", false);

	   std::cout << "Name: " << name << std::endl;
	   std::cout << "Version: " << version << std::endl;
	   std::cout << "Debug mode: " << std::boolalpha << debug << std::endl;

	   return;
	}
	
	
};
	