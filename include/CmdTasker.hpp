#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace nlohmann;

class CmdTasker{
	private:
		string 	ps_configPath, pt_configPath;
		json 	ps_configData, pt_configData;
	public:
	
	CmdTasker(string ps_configPath, string pt_configPath): ps_configPath(ps_configPath), pt_configPath(pt_configPath) {
		loadConfigs();
	}
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
	
	/**
	 * Volgorde options maakt soms uit, dus let hierop bij de config.
	 */
	void addStaticFlags(std::string& command, json& dt_config_data){
		for(auto option:dt_config_data["static_options"]){
			for (auto& [flag, value] : option.items()) {
                command = command + " " + flag;
                // If the value is a non‑empty string, append it as an argument
                if (value.is_string() && !value.get<string>().empty()) {
                    command = command + " " + value.get<string>();
                } // Handle other types (numbers, bool)
            }
		}
	}
	
	/**
	 * Volgorde options maakt soms uit, dus let hierop bij de config.
	 */
	void addStaticPreFlags(string& command, json& dt_config_data){
		for(auto option:dt_config_data["static_pre_flags"]){
			for (auto& [flag, value] : option.items()) {
                command = command + " " + flag;
                // If the value is a non‑empty string, append it as an argument
                if (value.is_string() && !value.get<string>().empty()) {
                    command = command + " " + value.get<string>();
                } // Handle other types (numbers, bool)
            }
		}
	}
	
	void processPath(filesystem::path filePath, json& pt_config_data){
		string command = pt_config_data["binPath"].get<string>();
		addStaticPreFlags(command, pt_config_data);
		command = command + " " + filePath.c_str() + " ";		// binPath
		addStaticFlags(command, pt_config_data);
		command += 	" " + 
					pt_config_data["outputFlag"].get<string>() 		+ " " + 							// outputPath
					ps_configData["outputDir"].get<string>() 		+ 									
					pt_config_data["name"].get<string>() 			+ "/" ;
					
		if(ps_configData["output_ext"].get<string>().empty()){
			command += filePath.filename().string();
		} else {
			command += filePath.stem().string() + ps_configData["output_ext"].get<string>();
		}
					
		if(pt_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
		if(pt_config_data["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}
		cout << command << "\n";
		auto status = system(command.c_str());
	}
	
	void processDir(filesystem::path dirPath, json& pt_config_data){
		string command = pt_config_data["binPath"].get<string>() + " " + dirPath.string();
		addStaticPreFlags(command, pt_config_data);
		if(!pt_config_data["outputFlag"].get<string>().empty()){
			command += " " + 
			pt_config_data["outputFlag"].get<string>() 	+ " " +
			ps_configData["outputDir"].get<string>();	
			
			command += pt_config_data["name"].get<string>() + "/";
			if(pt_config_data["run_per_dir"]){
				command += to_string(pt_config_data["it_begin"].get<int>()) + "/";
			}			
			
		}
		addStaticFlags(command, pt_config_data);
		cout << command << "\n";
		auto status = std::system(command.c_str());
	}
	
	void startCodeGeneration(){
		cout << "Starting code generation and outputting to " << ps_configData["outputDir"] << "\n";
		string command;
		for(auto& toolcfg:pt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << " [" << toolcfg["it_begin"]  << "," << toolcfg["it_end"] << ")\n";

			filesystem::create_directories((ps_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
				command = toolcfg["binPath"].get<string>() 	+ " ";
				addStaticPreFlags(command, toolcfg);
				command +=	toolcfg["seedFlag"].get<string>() 	+ " " 	+ to_string(toolcfg["it_begin"].get<int>()) + " ";
					
				if(toolcfg["output_arg"].get<bool>()){
					command += toolcfg["outputFlag"].get<string>() + " " + ps_configData["outputDir"].get<string>();
					command += toolcfg["name"].get<string>() 		+ "/" 	+ to_string(toolcfg["it_begin"].get<int>()) + toolcfg["it_ext"].get<string>();
				}
					
				addStaticFlags(command, toolcfg);
				if(toolcfg["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
				if(toolcfg["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}

				std::cout << command << "\n";
				auto status = std::system(command.c_str());
				command.clear();
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	// TODO: Implement filesystem::is_* checks if necessary?
	void startProcesses(){
		cout << "Starting processes and outputting to " << ps_configData["outputDir"] << "\n";
		
		for(auto& toolcfg:pt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			filesystem::create_directories((ps_configData["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			cout << toolcfg["name"] << "\n";

			if(toolcfg["run_per_file"].get<bool>()){
				if(toolcfg["run_as_it"].get<bool>()){
					for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
						processPath(
							toolcfg["tgtDir"].get<string>() + 
							to_string(toolcfg["it_begin"].get<int>()) +
							toolcfg["it_ext"].get<string>(), 
							toolcfg);
					}
				} else {
					for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
						if(toolcfg["continue_checkpoint"].get<bool>()){
							auto dest2check = 	ps_configData["outputDir"].get<string>() 	+ 
												toolcfg["name"].get<string>()  				+
												"/";
							if(ps_configData["output_ext"].empty()){
								dest2check += entry.path().filename().string();
							} else {
								dest2check += entry.path().stem().string() + ps_configData["output_ext"].get<string>();
							}
							if(!filesystem::exists(dest2check)){
								processPath(entry, toolcfg);
							}
							cout << "Checkpoint skip \n";
						} else {
							processPath(entry, toolcfg);
						}
					}
				}
			} else if(toolcfg["run_per_dir"].get<bool>()){ // add this key to all configs
				// TODO: test
				if(toolcfg["run_as_it"].get<bool>()){
					for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
						processDir(toolcfg["tgtDir"].get<string>() + to_string(toolcfg["it_begin"].get<int>()), toolcfg);
					}
				} else {
					for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
						processDir(entry, toolcfg);
					}
				}
			} 
			else {
				// TODO: add checkpoint continuation
				processDir(toolcfg["tgtDir"], toolcfg);
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void saveJsonToFile(const string& path, const json& data) {
	    ofstream file(path);
	    if (!file.is_open()) {
	        cerr << "Error: Could not open file for writing: " << path << "\n";
	        return;
	    }
	    file << data.dump(2);  // pretty print indentation
	    cout << "Saved config to " << path << "\n";
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