#include <filesystem>
#include <string>
#include <unordered_map>
#include "../compare/MageEvalComparer.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		vector<filesystem::path> &inputDirs;
		filesystem::path &outputPath;
				
	public:
	CmdTasker(
		vector<filesystem::path> &inputDirs, 
		filesystem::path &outputPath
	) : 
	inputDirs(inputDirs), 
	outputPath(outputPath)
	{}

	void startProcesses(){
		cout << "Start processes \n";
		MageEvalComparer mec(inputDirs, outputPath);
		mec.startComparisons();
	}
};
	