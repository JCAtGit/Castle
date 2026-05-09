#include "nlohmann/json.hpp"
#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		const string cg_configPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/codebase-scaffolder/config/cg_config.json";
		const string cs_configPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/codebase-scaffolder/config/cs_config.json";
		json cg_config_data, cs_config_data;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream cg_config_file(cg_configPath);
			ifstream cs_config_file(cs_configPath);
		   	if (!(cg_config_file.is_open() && cs_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
		   		return;
		   	}
		   	try {
		    	cg_config_data = json::parse(cg_config_file);
				cs_config_data = json::parse(cs_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		// misschien naar helper?
		void saveJsonToFile(const std::string& path, const json& data) {
		    std::ofstream file(path);
		    if (!file.is_open()) {
		        std::cerr << "Error: Could not open file for writing: " << path << std::endl;
		        return;
		    }
		    file << data.dump(4);  // pretty print with 4 spaces indentation
		    std::cout << "Saved config to " << path << std::endl;
		}
	public:
	CmdTasker(){
		cout << "Setup CmdTasker.\n";
		loadConfigs();
	}
	
	void startCodeGeneration(){
		cout << "Starting code generation and outputting to " << cs_config_data["outputDir"] << "\n";
		string command;
		for(auto& toolcfg:cg_config_data["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << " [" << toolcfg["it_begin"]  << "," << toolcfg["it_end"] << ")\n";
			filesystem::create_directories((cs_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
				command = 
					toolcfg["binPath"].get<string>() 	+ " " 	+ 
					toolcfg["seedFlag"].get<string>() 	+ " " 	+ std::to_string(toolcfg["it_begin"].get<int>()) 	+ " " +
					toolcfg["outputFlag"].get<string>() + " \"" + cs_config_data["outputDir"].get<string>() 			+ 
					toolcfg["name"].get<string>() 		+ "/" 	+ std::to_string(toolcfg["it_begin"].get<int>())	+ ".c\"" + 					
					" > /dev/null 2>&1";
				// TODO: add static flags	
				
				if(toolcfg["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
				std::cout << command << "\n";
				auto status = std::system(command.c_str());
				command.clear();
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(cs_config_data["save_on_exit"].get<bool>()){
			cs_config_data["timestamp"] = time(NULL);
			saveJsonToFile(cs_configPath,cs_config_data);
			saveJsonToFile(cg_configPath,cg_config_data);
		}
	}
};
	