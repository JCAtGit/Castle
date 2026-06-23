
#include <filesystem>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	filesystem::path examplePath0 = "/example/path/to/parse/";
	filesystem::path examplePath1 = "/example/path/to/parse/1";
	cout << examplePath0.stem() << "\n";
	cout << examplePath1.stem() << "\n";
}