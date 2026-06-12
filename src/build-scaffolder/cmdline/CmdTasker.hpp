#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		string 	ps_configPath, pt_configPath;
		json 	ps_configData, pt_configData;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream bt_config_file(pt_configPath);
			ifstream bs_config_file(ps_configPath);
		   	if (!(bt_config_file.is_open() && bs_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << "\n";
		   		return;
		   	}
		   	try {
		    	pt_configData = json::parse(bt_config_file);
				ps_configData = json::parse(bs_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << "\n";
		    	return;
		  	}
		}
		
		void processPath(filesystem::path filepath, json& bt_config_data){
			string command = 
				bt_config_data["binPath"].get<string>() 	+ " " + filepath.c_str() + " " + 
				bt_config_data["outputFlag"].get<string>() 	+ " " + ps_configData["outputDir"].get<string>() + 
																	bt_config_data["name"].get<string>() + "/" +
																	filepath.stem().c_str() + ".o";			
			// TODO: add static flags
			if(bt_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
			if(bt_config_data["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}

			cout << command << "\n";
			auto status = system(command.c_str());
		}

		void saveJsonToFile(const std::string& path, const json& data) {
		    ofstream file(path);
		    if (!file.is_open()) {
		        std::cerr << "Error: Could not open file for writing: " << path << "\n";
		        return;
		    }
		    file << data.dump(2);  // pretty print with 4 spaces indentation
		    cout << "Saved config to " << path << "\n";
		}
	public:
	CmdTasker(string bs_configPath, string bt_configPath):ps_configPath(bs_configPath), pt_configPath(bt_configPath){
		loadConfigs();
	}
	
	void startBuildProcesses(){
		cout << "Starting build processes and outputting to " << ps_configData["outputDir"] << "\n";
		for(auto& toolcfg:pt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << "\n";
			filesystem::create_directories((ps_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			
			if(toolcfg["run_per_file"].get<bool>()){
				if(toolcfg["run_as_it"].get<bool>()){
					for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
						filesystem::path entry = toolcfg["tgtDir"].get<string>() + std::to_string(toolcfg["it_begin"].get<int>()) + ".c ";											+ ".c " ;
						processPath(entry, toolcfg);
					}
				} else {
					for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
						if(entry.path().extension() == ".c"){
							processPath(entry, toolcfg);
						}
					}
				}
			} else {
				// TODO: add static flags, etc. Currently assumes that separate scripts handles everything
				string command = toolcfg["binPath"].get<string>();
				std::cout << command << "\n";
				auto status = std::system(command.c_str());
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(ps_configData["save_on_exit"].get<bool>()){
			cout << "Persist changes." << "\n";
			pt_configData["timestamp"] = time(NULL);
			saveJsonToFile(ps_configPath,ps_configData);
			saveJsonToFile(pt_configPath, pt_configData);
		}
	}
};
	