// normalize_path_demo.cpp
// Compile: g++ -std=c++17 normalize_path_demo.cpp -o norm

#include <iostream>
#include <filesystem>
#include <string>
#include "../../../include/helper.hpp"
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::string input;
    if (argc > 1)
        input = argv[1];
    else {
        std::cout << "Enter a path: ";
        std::getline(std::cin, input);
    }

    if (input.empty()) return 1;

    fs::path p(input);
    fs::path abs_absolute = fs::absolute(p);
    fs::path abs_normalized = fs::weakly_canonical(p);

    std::cout << "Input          : " << p << "\n";
    std::cout << "absolute       : " << abs_absolute << "\n";
    std::cout << "weakly_canonical: " << abs_normalized << "\n";
	
	if (argc > 2)
		std::cout << "rel_path: " << rel_outputPath(argv[1], argv[2]) << "\n";
    return 0;
}