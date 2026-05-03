#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
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
		
		using voidFn = std::function<void(filesystem::path, json&)>;
		void processFile(filesystem::path filepath, json& bt_config_data){
			if(filesystem::is_regular_file(filepath) && (filepath.extension() == ".c")){
				string command = 
					bt_config_data["binPath"].get<string>() 	+ " " + filepath.c_str() + " " + 
					bt_config_data["outputFlag"].get<string>() 	+ " " + bs_config_data["outputDir"].get<string>() + 
																		bt_config_data["name"].get<string>() + "/" +
																		filepath.stem().c_str() + ".o" +
					" > /dev/null 2>&1";
				if(bt_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
				std::cout << command << "\n";
				auto status = std::system(command.c_str());
			}
		}
		void processFolder(filesystem::path filepath, json& bt_config_data){}
		void processMixed(filesystem::path filepath, json& bt_config_data){}
		void processCustom(filesystem::path filepath, json& bt_config_data){}
		
		voidFn selectProcessor(const std::string& choice) {
	        if (choice == "file")   return [this](filesystem::path p, json j) { processFile(p, j); };
	        if (choice == "folder") return [this](filesystem::path p, json j) { processFolder(p, j); };
	        if (choice == "mixed")  return [this](filesystem::path p, json j) { processMixed(p, j); };
	        if (choice == "custom") return [this](filesystem::path p, json j) { processCustom(p, j); };
			cout << "invalid choice for processor";
	        return nullptr;
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
	CmdTasker(){
		loadConfigs();
	}
	
	void startBuildProcesses(){
		cout << "Starting build processes and outputting to " << bs_config_data["outputDir"] << "\n";
		voidFn processor;
		for(auto& toolcfg:bt_config_data["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << "\n";
			filesystem::create_directories((bs_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			processor = selectProcessor(toolcfg["mode"].get<string>());
			for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
				processor(entry, toolcfg);
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(bs_config_data["save_on_exit"].get<bool>()){
			cout << "Persist changes." << "\n";
			bt_config_data["timestamp"] = time(NULL);
			saveJsonToFile(bs_configPath,bs_config_data);
			saveJsonToFile(bt_configPath, bt_config_data);
		}
	}
};
	