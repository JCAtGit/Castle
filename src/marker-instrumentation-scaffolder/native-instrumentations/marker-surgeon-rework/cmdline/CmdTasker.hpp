#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:
	string inputPath, outputPath, mrkrFormatsPath;
	json markerFormats; 
	
	void loadConfigs(){
		cout << "Importing JSON configs.\n";
	   	ifstream markerFormatsFile(mrkrFormatsPath);
	   	if (!markerFormatsFile.is_open()) {
	    	cerr << "Error: Could not open required config files, see the source json folder for details." << endl;
	   		return;
	   	}
	   	try {
	    	markerFormats = json::parse(markerFormatsFile);
	   	} catch (const json::parse_error& e) {
	    	cerr << "Parse error: " << e.what() << endl;
	    	return;
	  	}
	}
	
	public:
	CmdTasker(string inputPath, string outputPath, string mrkrFormatsPath): inputPath(inputPath), outputPath(outputPath), mrkrFormatsPath(mrkrFormatsPath) {
		loadConfigs();
	}
	
	
};

