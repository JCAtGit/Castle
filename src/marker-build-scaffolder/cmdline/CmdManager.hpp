#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdManager{
	private:
		const string bt_configPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/marker-build-scaffolder/config/bt_config.json";
		const string bs_configPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/marker-build-scaffolder/config/bs_config.json";
		json bt_config_data, bs_config_data;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream bt_config_file(bt_configPath);
			ifstream bs_config_file(bs_configPath);
		   	if (!(bt_config_file.is_open() && bs_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
		   		return;
		   	}
		   	try {
		    	bt_config_data = json::parse(bt_config_file);
				bs_config_data = json::parse(bs_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
	public:
	CmdManager(){
		loadConfigs();
	}
	
	using voidFn = std::function<void()>;
	void processFile(){}
	void processFolder(){}
	void processMixed(){}
	void processCustom(){}
	voidFn selectProcessor(const std::string& choice) {
        if (choice == "file")   return [this]() { processFile(); };
        if (choice == "folder") return [this]() { processFolder(); };
        if (choice == "mixed")  return [this]() { processMixed(); };
        if (choice == "custom") return [this]() { processCustom(); };
        return nullptr;
    }
	
	void startBuildProcesses(){
		cout << "Starting build processes and outputting to " << bs_config_data["outputDir"] << "\n";
		string command;
		voidFn processor;
		for(auto& toolcfg:bt_config_data["buildTargets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			filesystem::create_directories((bs_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			processor = selectProcessor(toolcfg["mode"].get<string>());
			for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
				// ...
			}
		}
	}
	
	void startCodeGeneration(){
		cout << "Starting code generation and outputting to " << cs_config_data["outputDir"] << "\n";
		string command;
		for(auto& toolcfg:cg_config_data["code_generators"]){
			cout << toolcfg["name"] << " [" << toolcfg["it_begin"]  << "," << toolcfg["it_end"] << ")\n";
			filesystem::create_directories((cs_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
				command = 
					toolcfg["binPath"].get<string>() 	+ " " 	+ 
					toolcfg["seedFlag"].get<string>() 	+ " " 	+ std::to_string(toolcfg["it_begin"].get<int>()) 	+ " " +
					toolcfg["outputFlag"].get<string>() + " \"" + cs_config_data["outputDir"].get<string>() 			+ 
					toolcfg["name"].get<string>() 		+ "/" 	+ std::to_string(toolcfg["it_begin"].get<int>())	+ 
					".c\"";
				std::cout << command << "\n";
				auto status = std::system(command.c_str());
				command.clear();
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}

};
	