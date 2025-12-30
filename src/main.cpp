#include "raylib.h"
#include "Simulation.h"
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

    // create the Simulation Instance
    Simulation simulation(config.screenWidth, config.screenHeight, cellSize);

    // variables for input
    ParticleType currentType = SAND;
    bool isCursorHidden = false;

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_H)) {
            isCursorHidden = !isCursorHidden;
            if (isCursorHidden) HideCursor(); else ShowCursor();
        }

        if (IsKeyPressed(KEY_Q)) currentType = SAND;
        if (IsKeyPressed(KEY_W)) currentType = STONE;
        if (IsKeyPressed(KEY_E)) currentType = WATER;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            int gridX = (int)mousePos.x / cellSize;
            int gridY = (int)mousePos.y / cellSize;

            simulation.SetParticle(gridX, gridY, currentType);
        }

        simulation.Update();


        BeginDrawing();
        ClearBackground(BLACK); // changed to black for better contrast

        // draw the Grid
        const auto& grid = simulation.GetGrid();
        int rows = simulation.GetRows();
        int cols = simulation.GetCols();

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                if (grid[y][x] != EMPTY) {
                    Color color = BLACK;
                    if (grid[y][x] == SAND) color = YELLOW;
                    else if (grid[y][x] == STONE) color = DARKGRAY;
                    else if (grid[y][x] == WATER) color = BLUE;

                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
                }
            }
        }

        // UI Text
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);
        if (currentType == SAND) DrawText("Selected: SAND", 10, 40, 20, YELLOW);
        else if (currentType == STONE) DrawText("Selected: STONE", 10, 40, 20, GRAY);
        else if (currentType == WATER) DrawText("Selected: WATER", 10, 40, 20, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}