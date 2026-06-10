#include <filesystem>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "helper.hpp"

using namespace nlohmann;
using namespace std;

class CmdTasker{
	private:		
	
	public:
	CmdTasker(
		filesystem::path inputPath, 
		filesystem::path oracleDir, 
		filesystem::path outputPath, 
		filesystem::path mrkrFormatsPath,
		bool& nameEncodedMarkers
	){}
};
	