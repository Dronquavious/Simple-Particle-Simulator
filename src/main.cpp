#include "raylib.h"
#include "Simulation.h"
#include "Renderer.h"
#include "UI.h"
#include <iostream>
#include <fstream>
#include <string>

// --- Config Loading (will move this to Utils.h later) ---
struct Config {
    int screenWidth;
    int screenHeight;
    int cellSize;
};

Config loadConfig(const std::string& filename) {
    Config config = { 1280, 720, 20 }; // default values
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

            // simple whitespace trim
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

// --- Main Entry Point ---
int main() {
    // load Settings
    Config config = loadConfig("config.ini");
    int cellSize = config.cellSize;

    // setup Window
    InitWindow(config.screenWidth, config.screenHeight, "Advanced Particle Sim");
    SetTargetFPS(60);

    // Instances
    Simulation simulation(config.screenWidth, config.screenHeight, cellSize);
    Renderer renderer(cellSize);
    UI ui(config.screenWidth, config.screenHeight);

    // variables for input
    ParticleType currentType = SAND;
    bool isCursorHidden = false;

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_H)) {
            isCursorHidden = !isCursorHidden;
            if (isCursorHidden) HideCursor(); else ShowCursor();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ui.IsMouseOverUI()) {
            Vector2 mousePos = GetMousePosition();
            int gridX = (int)mousePos.x / cellSize;
            int gridY = (int)mousePos.y / cellSize;
            simulation.SetParticle(gridX, gridY, currentType);
        }

        simulation.Update();


        BeginDrawing();
        ClearBackground(BLACK); // changed to black for better contrast

        renderer.Draw(simulation);

        // draw the UI on TOP of the game
        // returns the new selected type (or the old one if nothing changed)
        currentType = ui.Draw(currentType);

        // UI Text
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}