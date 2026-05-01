#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <filesystem>
namespace fs = std::filesystem;

//#include "sha3/sha3_256.hpp"
#include "picosha2.h"

class FingerPrinter{
	private:
	std::string compute_file_hash(const std::string& path) {
	    // Open file in binary mode
	    std::ifstream file(path, std::ios::binary);
	    if (!file) {
	        // Handle error – return empty string or throw
	        return {};
	    }

	    // Read entire file into a string
	    std::string content((std::istreambuf_iterator<char>(file)),
	                         std::istreambuf_iterator<char>());

	    // Compute hash
	    return picosha2::hash256_hex_string(content);
	}
	public:
		json getFingerprintMap(fs::path path){
			json fingerprint = {};
			
			if (fs::is_directory(path)) {
				return {};
			} else if(fs::is_regular_file(path)){
				
			} else {
				return {};
			}
			
			return fingerprint ;
		}
		
	
};