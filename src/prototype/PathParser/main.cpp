
#include <filesystem>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void exampleParser(string output){
	std::vector<std::string> comments;
	std::istringstream iss(output);
	std::string line;
	while (std::getline(iss, line)) {
	    if (line.find('[') == 0) {
	        size_t start = line.find(']') + 2; // skip "] "
	        if (start < line.size())
	            comments.push_back(line.substr(start));
	    }
	}
	
	cout << "Parsed lines:\n";
	for(auto c:comments){
		cout << c << "\n"; 
	}
}

string execCommandAndCaptureOutput(const string& cmd, string& output) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    return output;
}

int main(int argc, char* argv[]) {
	filesystem::path examplePath0 = "/example/path/to/parse/";
	filesystem::path examplePath1 = "/example/path/to/parse/1";
	cout << examplePath0.stem() << "\n";
	cout << examplePath1.stem() << "\n";
	
	cout << examplePath0.parent_path() << "\n";
	cout << examplePath1.parent_path() << "\n";
	
	cout << examplePath0.parent_path().stem() << "\n";
	cout << examplePath1.parent_path().filename() << "\n";
	
	cout << examplePath0.parent_path().parent_path() << "\n";
	
	string output;
	string command = 
		"llvm-readelf -p .comment \
		/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/csmith_mage_clang/999.o  	\
		";
	execCommandAndCaptureOutput(command, output);
	exampleParser(output);
}