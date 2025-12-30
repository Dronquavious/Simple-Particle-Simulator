#pragma once
#include <string>
#include <fstream>
#include <iostream>

struct Config {
    int screenWidth;
    int screenHeight;
    int cellSize;
};

// make this "inline" so we can define the function in the header 
// without needing a .cpp file for just one function.
inline Config LoadConfig(const std::string& filename) {
    Config config = { 1600, 900, 20 }; // updated defaults
    std::ifstream configFile(filename);

    if (!configFile.is_open()) {
        std::cerr << "Warning: Could not open config file. Using defaults." << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string valueStr = line.substr(delimiterPos + 1);

            // trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            valueStr.erase(0, valueStr.find_first_not_of(" \t"));
            valueStr.erase(valueStr.find_last_not_of(" \t") + 1);

            try {
                int value = std::stoi(valueStr);
                if (key == "screenWidth") config.screenWidth = value;
                else if (key == "screenHeight") config.screenHeight = value;
                else if (key == "cellSize") config.cellSize = value;
            }
            catch (...) {}
        }
    }
    return config;
}