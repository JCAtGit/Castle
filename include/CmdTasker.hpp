#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

/**
 *	A class that selects and schedules tasks to process based on the input tool and scaffolder configurations.
 *	It has multiple enqueuement procedures given some configuration:
 *		
 */
class CmdTasker {
	private:
		string 	ps_configPath, pt_configPath; // paths to the scaffolder and tool config json file.
		json 	ps_config_data, pt_configData;
	public:
	
	/**
	 * 	Constructor that accepts the tool and scaffolder json paths.
	 * 	@param	ps_configPath	path leading to the scaffolder config json file.
	 * 	@param	pt_configPath	path leading to the tool	   config json file.
	 */
	CmdTasker(string ps_configPath, string pt_configPath): ps_configPath(ps_configPath), pt_configPath(pt_configPath) {
		loadConfigs();
	}
	
	/**
	 *	Method to load the configs into their respective variables.
	 */
	void loadConfigs(){
		cout << "Importing JSON configs.\n";
	   	ifstream pt_config_file(pt_configPath);
		ifstream ps_config_file(ps_configPath);
	   	if (!(pt_config_file.is_open() && ps_config_file.is_open())) {
	    	cerr << "Error: Could not open required config files, see the source json folder for details." << "\n";
	   		return;
	   	}
	   	try {
	    	pt_configData = json::parse(pt_config_file);
			ps_config_data = json::parse(ps_config_file);
	   	} catch (const json::parse_error& e) {
	    	cerr << "Parse error: " << e.what() << "\n";
	    	return;
	  	}
	}
	
	/**
	 * 	Adds config-specified static flags to the pipeline command, appends after first argument.
	 *	@param command 			the invoked command by the pipeline
	 * 	@param dt_config_data	tool config data
	 */
	void addStaticFlags(string& command, json& dt_config_data){
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
	 *  Adds config-specified static flags to the pipeline command, appends before first argument.
	 *	@param command 			the invoked command by the pipeline
 	 * 	@param dt_config_data	tool config data
	 */
	void addStaticPreFlags(string& command, json& pt_config_data){
		for(auto option:pt_config_data["static_pre_flags"]){
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
	 *	Invokes the pipeline command on the target file within the tool configuration json.
	 * 	@param filePath			target file element on which the command is invoked
	 *	@param pt_config_data	tool config data
	 */
	void processFile(filesystem::path filePath, json& pt_config_data){
		string command = pt_config_data["binPath"].get<string>();
		addStaticPreFlags(command, pt_config_data);
		command = command + " " + filePath.c_str() + " ";		// binPath
		addStaticFlags(command, pt_config_data);
		command += 	" " + 
					pt_config_data["outputFlag"].get<string>() 		+ " " + 							// outputPath
					ps_config_data["outputDir"].get<string>() 		+ 									
					pt_config_data["name"].get<string>() 			+ "/" ;
					
		if(ps_config_data["output_ext"].is_null()){
			command += filePath.filename().string();
		} else {
			command += filePath.stem().string() + ps_config_data["output_ext"].get<string>();
		}
					
		if(pt_config_data["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
		if(pt_config_data["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}
		cout << command << "\n";
		auto status = system(command.c_str());
	}
	
	/**
	 *	Invokes the pipeline command on the target dir within the tool configuration json.
	 *	@param dirPath			target dir element on which the command is invoked
	 *	@param pt_config_data	tool config data
	 */
	void processDir(filesystem::path dirPath, json& pt_config_data){
		string command = pt_config_data["binPath"].get<string>() + " " + dirPath.string();
		addStaticPreFlags(command, pt_config_data);
		if(!pt_config_data["outputFlag"].get<string>().empty()){
			command += " " + 
			pt_config_data["outputFlag"].get<string>() 	+ " " +
			ps_config_data["outputDir"].get<string>();	
			
			command += pt_config_data["name"].get<string>() + "/";
			if(pt_config_data["run_per_dir"]){
				command += to_string(pt_config_data["it_begin"].get<int>()) + "/";
			}			
			
		}
		addStaticFlags(command, pt_config_data);
		cout << command << "\n";
		auto status = std::system(command.c_str());
	}
	
	/**
	 *	Entry method for tasker activities.
	 *	Method utilized solely by the Codebase Scaffolder to create files with code-generators using some seed.
	 */
	void startCodeGeneration(){
		cout << "Starting code generation and outputting to " << ps_config_data["outputDir"] << "\n";
		string command;
		for(auto& toolcfg:pt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			cout << toolcfg["name"] << " [" << toolcfg["it_begin"]  << "," << toolcfg["it_end"] << ")\n";

			filesystem::create_directories((ps_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
				command = toolcfg["binPath"].get<string>() 	+ " ";
				addStaticPreFlags(command, toolcfg);
				command +=	toolcfg["seedFlag"].get<string>() 	+ " " 	+ to_string(toolcfg["it_begin"].get<int>()) + " ";
					
				if(toolcfg["output_arg"].get<bool>()){
					command += toolcfg["outputFlag"].get<string>() + " " + ps_config_data["outputDir"].get<string>();
					command += toolcfg["name"].get<string>() 		+ "/" 	+ to_string(toolcfg["it_begin"].get<int>()) + toolcfg["it_ext"].get<string>();
				}
					
				addStaticFlags(command, toolcfg);
				if(toolcfg["run_bin_as_shell"].get<bool>()){command = "bash " + command;}
				if(toolcfg["redirectNull"].get<bool>()){command = command + " > /dev/null 2>&1";}

				cout << command << "\n";
				auto status = system(command.c_str());
				command.clear();
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	/**
	 *	Entry method for tasker activities.
	 * 	Conducts general process activities to iterate in the selected manner over the contents of the target.
	 */
	void startProcesses(){
		cout << "Starting processes and outputting to " << ps_config_data["outputDir"] << "\n";
		for(auto& toolcfg:pt_configData["targets"]){
			if(!toolcfg["active"].get<bool>()){continue;}
			filesystem::create_directories((ps_config_data["outputDir"].get<string>() + toolcfg["name"].get<string>()));
			cout << toolcfg["name"] << "\n";

			// run_per_file mode and with submodes
			if(toolcfg["run_per_file"].get<bool>()){
				if(toolcfg["run_as_it"].get<bool>()){
					for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
						processFile(
							toolcfg["tgtDir"].get<string>() + 
							to_string(toolcfg["it_begin"].get<int>()) +
							toolcfg["it_ext"].get<string>(), 
							toolcfg);
					}
				} else {
					for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
						if(toolcfg["continue_checkpoint"].get<bool>()){
							auto dest2check = 	ps_config_data["outputDir"].get<string>() 	+ 
												toolcfg["name"].get<string>()  				+
												"/";
							if(ps_config_data["output_ext"].empty()){
								dest2check += entry.path().filename().string();
							} else {
								dest2check += entry.path().stem().string() + ps_config_data["output_ext"].get<string>();
							}
							if(!filesystem::exists(dest2check)){
								processFile(entry, toolcfg);
							}
							cout << "Checkpoint skip \n";
						} else {
							processFile(entry, toolcfg);
						}
					}
				}
			} else if(toolcfg["run_per_dir"].get<bool>()){
				if(toolcfg["run_as_it"].get<bool>()){
					for(;toolcfg["it_begin"].get<int>() < toolcfg["it_end"].get<int>(); toolcfg["it_begin"] = (toolcfg["it_begin"].get<int>() + 1)){
						processDir(toolcfg["tgtDir"].get<string>() + to_string(toolcfg["it_begin"].get<int>()), toolcfg);
					}
				} else {
					for (const auto& entry : filesystem::directory_iterator(toolcfg["tgtDir"])) {
						processDir(entry, toolcfg);
					}
				}
			} else {
				processDir(toolcfg["tgtDir"], toolcfg);
			}
			toolcfg["timestamp"] = time(NULL);
		}
	}
	
	/**
	 *	Commits time of last tool utilization and iteration to file.
	 */
	void persistChanges(){
		if(ps_config_data["save_on_exit"].get<bool>()){
			cout << "Persist changes." << "\n";
			pt_configData["timestamp"] = time(NULL);
			saveJsonToFile(ps_configPath,ps_config_data);
			saveJsonToFile(pt_configPath, pt_configData);
		}
	}
};