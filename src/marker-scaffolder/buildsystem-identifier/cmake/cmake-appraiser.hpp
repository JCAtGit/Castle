#include <map>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <regex>
#include <iostream>

class CMakeAppraiser {
	std::unordered_map<std::string, std::vector<std::string>> extMap = {
			{".CMake", {}},
			{".cmake-version",{}}
			
	};
	
	// Common files.
	std::unordered_map<std::string, std::vector<std::string>> fileMap = {
		{"cmake_install.cmake",{}},
		{"compile_commands.json",{}},
		{"CMakeCache.txt",{}},
		{"CMakeLists.txt",{}},
		{"CMakePresets.json, ",{}},
		{"CMakeUserPresets.json",{}},
		{"CPackConfig.cmake",{}},
		{"CPackSourceConfig.cmake",{}},
		{"",{}},
	};
	
	std::unordered_map<std::string, std::string> dirMap = {
		{"CMake",{}}	
	};

	void searchIncludesFromCMakeLists(std::string CMakeListsText){
		std::smatch match; 
		std::regex self_regex(
			"^include[(](.*)[)]$", 
			std::regex_constants::ECMAScript | 
			std::regex_constants::icase
	);
		
		std::sregex_iterator it(CMakeListsText.begin(), CMakeListsText.end(), self_regex);
		std::sregex_iterator end;

		for (; it != end; ++it) {
		    std::cout << "Full: " << it->str(0) << "\n";
		    std::cout << "Inside: " << it->str(1) << "\n";
		}
	}
};