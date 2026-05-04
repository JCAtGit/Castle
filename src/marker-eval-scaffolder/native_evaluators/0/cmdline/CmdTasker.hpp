#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include "../../../../../include/helper.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		string inputPath, outputPath, mode, mrkrCfgPath;
		json mrkrCfg;
		
		using fn = std::function<void(filesystem::path, json&)>;
		// TODO
		void processFile(filesystem::path filepath, json& et_config_data){
			// general: retrieve file and build (recovery) AST
			// 1. retrieve file and load to string
			string filedata = file2string(inputPath);
			// 2. build AST from stringdata
			
		}
		void processFolder(filesystem::path filepath, json& et_config_data){}
		void processMixed(filesystem::path filepath, json& et_config_data){}
		void processCustom(filesystem::path filepath, json& et_config_data){}
		
		fn selectProcessor() {
	        if (mode == "file")   return [this](filesystem::path p, json j) -> void { processFile(p, j); };
	        if (mode == "folder") return [this](filesystem::path p, json j) -> void { processFolder(p, j); };
	        if (mode == "mixed")  return [this](filesystem::path p, json j) -> void { processMixed(p, j); };
	        if (mode == "custom") return [this](filesystem::path p, json j) -> void { processCustom(p, j); };
			cout << "invalid choice for processor";
	        return nullptr;
	    }
		
	public:
	CmdTasker(string inputPath, string outputPath, string  mode, string  mrkrCfgPath): 
			inputPath(inputPath), 
			outputPath(outputPath),
			mode(mode),
			mrkrCfgPath(mrkrCfgPath)
	{}
	
	// TODO
	void startEvaluationProcesses(){
		// select processor based on oracle retrieval
		fn processor = selectProcessor();
		
		// select 
	}
};
	