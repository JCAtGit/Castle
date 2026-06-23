#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "json.hpp"

using namespace nlohmann;
using namespace std;

vector<string> pipelineTasks = {};

/**
 * 	Load some pipeline config
 */

class CmdTasker{
	private:
		string 	ps_configPath, pt_configPath;
		json 	ps_configData, pt_configData;
		
	public:
	CmdTasker(string bs_configPath, string bt_configPath):ps_configPath(bs_configPath), pt_configPath(bt_configPath){
	}

};
	