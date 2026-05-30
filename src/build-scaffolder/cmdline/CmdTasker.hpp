#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		string 	bs_configPath, bt_configPath;
		json 	bs_configData, bt_configData;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream bt_config_file(bt_configPath);
			ifstream bs_config_file(bs_configPath);
		   	if (!(bt_config_file.is_open() && bs_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
		   		return;
		   	}
		   	try {
		    	bt_configData = json::parse(bt_config_file);
				bs_configData = json::parse(bs_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		void processPath(filesystem::path filepath, json& bt_config_data){
			string command = 
				bt_config_data["binPath"].get<string>() 	+ " " + filepath.c_str() + " " + 
				bt_config_data["outputFlag"].get<string>() 	+ " " + bs_configData["outputDir"].get<string>() + 
																	bt_config_data["name"].get<string>() + "/" +
																	filepath.stem().c_str() + ".o" +
				" > /dev/null 2>&1";
			// TODO: add static flags
			if(bt_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
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
	CmdTasker(string bs_configPath, string bt_configPath):bs_configPath(bs_configPath), bt_configPath(bt_configPath){
		loadConfigs();
	}
	
	void startBuildProcesses(){
		cout << "Starting build processes and outputting to " << bs_configData["outputDir"] << "\n";
		for(auto& toolcfg:bt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << "\n";
			filesystem::create_directories((bs_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			
			if(toolcfg["run_per_file"].get<bool>()){
				for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
					if(entry.path().extension() == ".c"){
						processPath(entry, toolcfg);
					}
				}
			} else {
				string command = toolcfg["binPath"].get<string>();
				// TODO: add static flags
				std::cout << command << "\n";
				auto status = std::system(command.c_str());
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(bs_configData["save_on_exit"].get<bool>()){
			cout << "Persist changes." << "\n";
			bt_configData["timestamp"] = time(NULL);
			saveJsonToFile(bs_configPath,bs_configData);
			saveJsonToFile(bt_configPath, bt_configData);
		}
	}
};
	