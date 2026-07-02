#pragma once
#include <iostream>
#include <string>

using namespace std;

/**
 *	Executes a shell command and captures its output in a string.
 *	@param	cmd		String representing the command to be executed.
 *	@param	output	String holding the result of the command after execution.
 */
string execCommandAndCaptureOutput(const string& cmd, string& output) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    return output;
}