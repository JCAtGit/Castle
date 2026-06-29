#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <array>

using namespace std;

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