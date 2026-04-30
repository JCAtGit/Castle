#include <map>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

namespace fs = std::filesystem;

class BuildSystemProcessor{
	std::unordered_map<std::string, std::string> CMakeFileMap;
	std::unordered_map<std::string, std::string> MakeFileMap;
	private:
		/**
		 * Stores path results found to indicate build systems.
		 */
		struct searchResults {
			std::vector<std::string> CMake;
			std::vector<std::string> Make;
		} sr;
		
	public:
	
	/**
	 * 	Determines build system by the presence of files.
	 */
	std::string identifyBuildSystem(fs::path dir){
		
		for (const auto& entry : fs::recursive_directory_iterator(dir)) {
			
		}
	}
};