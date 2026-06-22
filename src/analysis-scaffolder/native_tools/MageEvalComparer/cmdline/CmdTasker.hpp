#include <filesystem>
#include <string>
#include <unordered_map>
#include "../compare/MageEvalComparer.hpp"

using namespace nlohmann;
using namespace std;

// TODO: blablabla SE-principles: extraction, refactor and yada yada yada.
class CmdTasker{
	private:		
		vector<filesystem::path> &inputDirs;
		filesystem::path &outputPath;							// use outputPath as dir to retrieve previously existing aggregated data from.
				
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
	