#include <filesystem>
#include <string>
#include <unordered_map>
#include "../../../native-tools/MageEvalComparer/compare/MageEvalComparer.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
		vector<filesystem::path> 	&inputDirs;
		filesystem::path 			&outputPath,
									&buildScaffolderDir;
									
		bool 						&recombine_only;
	public:
	CmdTasker(
		vector<filesystem::path> &inputDirs, 
		filesystem::path &outputPath,
		filesystem::path &buildScaffolderDir,
		bool &recombine_only
	) : 
	inputDirs(inputDirs), 
	outputPath(outputPath),
	recombine_only(recombine_only),
	buildScaffolderDir(buildScaffolderDir)
	{}

	void startProcesses(){
		cout << "Start processes \n";
		MageEvalComparer mec(inputDirs, outputPath, buildScaffolderDir);

		if(recombine_only){
			mec.startAggregationAndExport(recombine_only);
		} else {
			mec.gatherBinaryCharacteristics();
			mec.buildZPooledSummaries();
			mec.startAggregationAndExport(recombine_only);
		}
	}
};
	