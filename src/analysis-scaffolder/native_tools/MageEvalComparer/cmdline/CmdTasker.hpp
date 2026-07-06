#include <filesystem>
#include <string>
#include <unordered_map>
#include "../compare/MageEvalComparer.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		vector<filesystem::path> 	&inputDirs;
		filesystem::path 			&outputPath;
		bool 						&recombine_only;
	public:
	CmdTasker(
		vector<filesystem::path> &inputDirs, 
		filesystem::path &outputPath,
		bool &recombine_only
	) : 
	inputDirs(inputDirs), 
	outputPath(outputPath),
	recombine_only(recombine_only)
	{}

	void startProcesses(){
		cout << "Start processes \n";
		MageEvalComparer mec(inputDirs, outputPath);

		if(recombine_only){
			mec.startComparisons(recombine_only);
		} else {
			mec.gatherBinaryCharacteristics();
			mec.buildZPooledSummaries();
			mec.startComparisons(recombine_only);
		}
	}
};
	