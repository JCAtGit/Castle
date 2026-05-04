#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
		const string et_configPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/marker-eval-scaffolder/config/et_config.json";
		const string es_configPath = "/home/user/eclipse-workspace/DE_CompilationBenchmarker/src/marker-eval-scaffolder/config/es_config.json";
		json et_config_data, es_config_data;
		
		void loadConfigs(){
			cout << "Importing JSON configs.\n";
		   	ifstream et_config_file(et_configPath);
			ifstream es_config_file(es_configPath);
		   	if (!(et_config_file.is_open() && es_config_file.is_open())) {
		    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
		   		return;
		   	}
		   	try {
		    	et_config_data = json::parse(et_config_file);
				es_config_data = json::parse(es_config_file);
		   	} catch (const json::parse_error& e) {
		    	cerr << "Parse error: " << e.what() << endl;
		    	return;
		  	}
		}
		
		/**
		 * Volgorde options maakt soms uit, dus let hierop bij de config.
		 */
		void addFlags(std::string& command, json& dt_config_data){
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
		
		using fn = std::function<void(filesystem::path, json&)>;
		// TODO
		void processFile(filesystem::path filepath, json& et_config_data){
			if(filesystem::is_regular_file(filepath)){
				string command = 
					et_config_data["binPath"].get<string>() 	+ " " + filepath.c_str() + " " + 
					et_config_data["outputFlag"].get<string>() 	+ " " + es_config_data["outputDir"].get<string>() + 
																		et_config_data["name"].get<string>() + "/" +
																		filepath.stem().c_str() + ".c";
				addFlags(command, et_config_data);
				if(et_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
				command = command + " > /dev/null 2>&1";
				std::cout << command << "\n";
				auto status = std::system(command.c_str());
			}
		}
		void processFolder(filesystem::path filepath, json& et_config_data){}
		void processMixed(filesystem::path filepath, json& et_config_data){}
		void processCustom(filesystem::path filepath, json& et_config_data){}
		
		fn selectProcessor(const std::string& choice) {
	        if (choice == "file")   return [this](filesystem::path p, json j) -> void { processFile(p, j); };
	        if (choice == "folder") return [this](filesystem::path p, json j) -> void { processFolder(p, j); };
	        if (choice == "mixed")  return [this](filesystem::path p, json j) -> void { processMixed(p, j); };
	        if (choice == "custom") return [this](filesystem::path p, json j) -> void { processCustom(p, j); };
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
	
	// TODO
	void startEvaluationProcesses(){
		cout << "Starting decompilation processes and outputting to " << es_config_data["outputDir"] << "\n";
		fn processor;
		for(auto& toolcfg:et_config_data["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << "\n";
			filesystem::create_directories((es_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			processor = selectProcessor(toolcfg["mode"].get<string>());
			for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
				processor(entry, toolcfg);
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	void persistChanges(){
		if(es_config_data["save_on_exit"].get<bool>()){
			et_config_data["timestamp"] = time(NULL);
			saveJsonToFile(es_configPath, es_config_data);
			saveJsonToFile(et_configPath, et_config_data);
		}
	}
};
	