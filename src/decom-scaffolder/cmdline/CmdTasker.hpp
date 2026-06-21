#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		string 	ds_configPath, dt_configPath;
		json 	ds_configData, dt_configData;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream dt_config_file(dt_configPath);
			ifstream ds_config_file(ds_configPath);
		   	if (!(dt_config_file.is_open() && ds_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << "\n";
		   		return;
		   	}
		   	try {
		    	dt_configData = json::parse(dt_config_file);
				ds_configData = json::parse(ds_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << "\n";
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
		
		void processPath(filesystem::path filepath, json& dt_config_data){
			string command = 
				dt_config_data["binPath"].get<string>() 	+ " " + filepath.c_str() + " " + 
				dt_config_data["outputFlag"].get<string>() 	+ " " + ds_configData["outputDir"].get<string>() + 
																	dt_config_data["name"].get<string>() + "/" +
																	filepath.stem().c_str() + ".c";
			addStaticFlags(command, dt_config_data);
			if(dt_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
			if(dt_config_data["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}
			cout << command << "\n";
			auto status = system(command.c_str());
		}
		
		void saveJsonToFile(const std::string& path, const json& data) {
		    ofstream file(path);
		    if (!file.is_open()) {
		        std::cerr << "Error: Could not open file for writing: " << path << "\n";
		        return;
		    }
		    file << data.dump(2);  // pretty print with indentation
		    std::cout << "Saved config to " << path << "\n";
		}
	public:
	CmdTasker(string ds_configPath, string dt_configPath): ds_configPath(ds_configPath), dt_configPath(dt_configPath) {
		loadConfigs();
	}
	
	void startDecompilationProcesses(){
		cout << "Starting decompilation processes and outputting to " << ds_configData["outputDir"] << "\n";
		for(auto& toolcfg:dt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			
			cout << toolcfg["name"] << "\n";
			filesystem::create_directories((ds_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
				// TODO: decompile entry has to be an executable, should have .o, no extension
				processPath(entry, toolcfg);
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(ds_configData["save_on_exit"].get<bool>()){
			dt_configData["timestamp"] = time(NULL);
			saveJsonToFile(ds_configPath, ds_configData);
			saveJsonToFile(dt_configPath, dt_configData);
		}
	}
};
	