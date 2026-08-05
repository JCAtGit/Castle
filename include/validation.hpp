#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <cctype>
#include <unordered_set>

using json = nlohmann::json;

/**
 * Parses objdump -d output line by line and updates a flat JSON status object.
 * 
 * @param markerStatus   A JSON object in the format:
 *                       { "0": false, "1": false, "2": false, ... }
 *                       Each key is a marker index. The value will be set to:
 *                       - true  if BOTH the label AND the call site exist in the objdump
 *                       - false otherwise (or if the key was not initially present, it stays absent)
 * @param objdumpOutput  The full disassembly text from `objdump -d`
 */
void validateMarkersFromObjdump(json& markerStatus, const std::string& objdumpOutput) {
    // Track which markers have a label and which have a call site
    std::unordered_set<int> labelsFound;
    std::unordered_set<int> callsFound;

    std::istringstream stream(objdumpOutput);
    std::string line;

    while (std::getline(stream, line)) {
        // -------------------------------------------------------------
        // Check for LABEL: "0000000000400480 <mage_alpha0>:"
        // -------------------------------------------------------------
        size_t labelPos = line.find("<mage_alpha");
        if (labelPos != std::string::npos) {
            size_t closeAngle = line.find('>', labelPos);
            if (closeAngle != std::string::npos &&
                closeAngle + 1 < line.size() &&
                line[closeAngle + 1] == ':') {
                
                // Extract the numeric index
                size_t numStart = labelPos + 12; // length of "<mage_alpha"
                std::string numStr;
                for (size_t i = numStart; i < closeAngle && std::isdigit(line[i]); ++i) {
                    numStr += line[i];
                }
                
                if (!numStr.empty()) {
                    int idx = std::stoi(numStr);
                    labelsFound.insert(idx);
                }
            }
        }

        // -------------------------------------------------------------
        // Check for CALL: "call   400480 <mage_alpha0>"
        // -------------------------------------------------------------
        size_t callPos = line.find("call");
        if (callPos != std::string::npos) {
            size_t markerPos = line.find("<mage_alpha", callPos);
            if (markerPos != std::string::npos) {
                size_t closeAngle = line.find('>', markerPos);
                if (closeAngle != std::string::npos) {
                    // Extract the numeric index
                    size_t numStart = markerPos + 12; // length of "<mage_alpha"
                    std::string numStr;
                    for (size_t i = numStart; i < closeAngle && std::isdigit(line[i]); ++i) {
                        numStr += line[i];
                    }
                    
                    if (!numStr.empty()) {
                        int idx = std::stoi(numStr);
                        callsFound.insert(idx);
                    }
                }
            }
        }
    }

    // -------------------------------------------------------------
    // Update the JSON object based on what was found
    // -------------------------------------------------------------
    for (auto& [key, value] : markerStatus.items()) {
        int idx = std::stoi(key);
        // Marker survives compilation ONLY if BOTH label AND call exist
        bool survives = (labelsFound.count(idx) > 0 && callsFound.count(idx) > 0);
        markerStatus[key] = survives;
    }
}