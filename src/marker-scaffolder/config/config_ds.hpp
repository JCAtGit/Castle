#pragma once
#include <map>
#include <vector>
#include <filesystem>

enum class CODE_ORIGIN_TYPE{
	SYNTHETIC, ORGANIC
};

enum class BUILD_MODE{
	GENERATE, BUILD, GENERATE_AND_BUILD, 
};

const std::map<std::string, CODE_ORIGIN_TYPE> mode_map{
	{"synthetic", CODE_ORIGIN_TYPE::SYNTHETIC},
	{"organic",   CODE_ORIGIN_TYPE::ORGANIC}
};

enum SYNTHETIC_CODE_GENS {
	CSMITH
};

// serves as boot and base config
struct config {
	unsigned long long timestamp;
	CODE_ORIGIN_TYPE mode = CODE_ORIGIN_TYPE::SYNTHETIC;
	std::vector<std::filesystem::path> buildTargets;
	std::filesystem::path outputFolder;
	int it_start;
	int it_cycles;
};
