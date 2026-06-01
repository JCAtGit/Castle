#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace nlohmann;

class CmdTasker{
	private:
		string 	cp_configPath, ct_configPath;
		json 	cp_configData, ct_configData;
	
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream ct_config_file(ct_configPath);
			ifstream cp_config_file(cp_configPath);
		   	if (!(ct_config_file.is_open() && cp_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << "\n";
		   		return;
		   	}
		   	try {
		    	ct_configData = json::parse(ct_config_file);
				cp_configData = json::parse(cp_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << "\n";
		    	return;
		  	}
		}
	public:
	
	CmdTasker(string cp_configPath, string ct_configPath):cp_configPath(cp_configPath), ct_configPath(ct_configPath){
		loadConfigs();
	}
	
	void startPipelines(){
		string command;
		for(auto pipeline: ct_configData["targets"]["pipeline"]){
			for(json item : pipeline.items()){
				cout << item["name"] << endl;
				// TODO: Voorlopig werkt deze alleen voor default settings, moet later ook externe configs gebruiken.
				command = 
					item["binPath"].get<string>() 	+ " " 	+ 
					" > /dev/null 2>&1";
//				// TODO: add static flags
//				
//				if(toolcfg["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
//				std::cout << command << "\n";
//				auto status = std::system(command.c_str());
				command.clear();
			}
		}
	}
};