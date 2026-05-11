#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		string 	es_configPath, et_configPath;
		json  	es_configData, et_configData;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream et_config_file(et_configPath);
			ifstream es_config_file(es_configPath);
		   	if (!(et_config_file.is_open() && es_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
		   		return;
		   	}
		   	try {
		    	et_configData = json::parse(et_config_file);
				es_configData = json::parse(es_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		/**
		 * Volgorde options maakt soms uit, dus let hierop bij de config.
		 */
		void addStaticFlags(std::string& command, json& dt_config_data){
			for(auto option:dt_config_data["static_options"]){
				for (auto& [flag, value] : option.items()) {
	                command = command + " " + flag;
	                // If the value is a non‑empty string, append it as an argument
	                if (value.is_string() && !value.get<std::string>().empty()) {
	                    command = command + " " + value.get<std::string>();
	                } // Handle other types (numbers, bool)
	            }
			}
		}
		
		void processPath(filesystem::path filepath, json& et_config_data){
			string command = 
				et_config_data["binPath"].get<string>() 	+ " " + filepath.c_str() + " " + 						// binPath
				et_config_data["oracleDir"].get<string>() 	+ " " + 												// oraclePath
				et_config_data["outputFlag"].get<string>() 	+ " " + es_configData["outputDir"].get<string>() 	+ 	// outputPath
																	et_config_data["name"].get<string>() + "/" 	+
																	filepath.stem().c_str() + ".c";
			addStaticFlags(command, et_config_data);
			if(et_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
			command = command + " > /dev/null 2>&1";
			std::cout << command << "\n";
			auto status = std::system(command.c_str());
		}
		
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
	CmdTasker(string es_configPath, string et_configPath): es_configPath(es_configPath), et_configPath(et_configPath){
		loadConfigs();
	}
	
	// TODO
	void startEvaluationProcesses(){
		cout << "Starting evaluation processes and outputting to " << es_configData["outputDir"] << "\n";
		for(auto& toolcfg:et_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << "\n";
			filesystem::create_directories((es_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
				if(entry.path().extension() == ".c"){
					processPath(entry, toolcfg);
				}
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(es_configData["save_on_exit"].get<bool>()){
			et_configData["timestamp"] = time(NULL);
			saveJsonToFile(es_configPath, es_configData);
			saveJsonToFile(et_configPath, et_configData);
		}
	}
};
	