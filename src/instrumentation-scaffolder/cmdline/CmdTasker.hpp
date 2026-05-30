#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		string 	is_configPath, it_configPath;
		json 	is_configData, it_configData;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream it_config_file(it_configPath);
			ifstream is_config_file(is_configPath);
		   	if (!(it_config_file.is_open() && is_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
		   		return;
		   	}
		   	try {
		    	it_configData = json::parse(it_config_file);
				is_configData = json::parse(is_config_file);
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
		
		/**
		 * Volgorde options maakt soms uit, dus let hierop bij de config.
		 */
		void addStaticPreFlags(std::string& command, json& dt_config_data){
			for(auto option:dt_config_data["static_pre_flags"]){
				for (auto& [flag, value] : option.items()) {
	                command = command + " " + flag;
	                // If the value is a non‑empty string, append it as an argument
	                if (value.is_string() && !value.get<std::string>().empty()) {
	                    command = command + " " + value.get<std::string>();
	                } // Handle other types (numbers, bool)
	            }
			}
		}
		
		void processPath(filesystem::path filePath, json& it_config_data){
			string command = it_config_data["binPath"].get<string>();
			addStaticPreFlags(command, it_config_data);
			command = command + " " + filePath.c_str() + " ";		// binPath
			addStaticFlags(command, it_config_data);
			command += 	" " + 
						it_config_data["outputFlag"].get<string>() 		+ " " + 							// outputPath
						is_configData["outputDir"].get<string>() 		+ 									
						it_config_data["name"].get<string>() 			+ "/" +
						filePath.filename().string();
			if(it_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
			if(it_config_data["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}
			std::cout << command << "\n";
			auto status = std::system(command.c_str());
		}

		void processDir(filesystem::path dirPath, json& it_config_data){
			string command = it_config_data["binPath"].get<string>() + " " + dirPath.string();
			if(!it_config_data["outputFlag"].get<string>().empty()){
				command += " " + 
				it_config_data["outputFlag"].get<string>() 	+ " " + 
				is_configData["outputDir"].get<string>()	+
				it_config_data["name"].get<string>() 		+ "/";
			}
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
	CmdTasker(string is_configPath, string it_configPath) : is_configPath(is_configPath), it_configPath(it_configPath){
		loadConfigs();
	}
	
	void startInstrumentationProcesses(){
		cout << "Starting instrumentation processes and outputting to " << is_configData["outputDir"] << "\n";
		
		for(auto& toolcfg:it_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			filesystem::create_directories((is_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			cout << toolcfg["name"] << "\n";

			if(toolcfg["run_per_file"]){
				for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
					processPath(entry, toolcfg);
				}
			} else {
				processDir(toolcfg["tgtDir"], toolcfg);
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(is_configData["save_on_exit"].get<bool>()){
			cout << "Persist changes." << "\n";
			it_configData["timestamp"] = time(NULL);
			saveJsonToFile(is_configPath,is_configData);
			saveJsonToFile(it_configPath, it_configData);
		}
	}
};
	