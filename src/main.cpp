#include "raylib.h"
#include "Simulation.h"
#include "Renderer.h"
#include "UI.h"
#include "Utils.h"

int main() {
    // load Settings
    Config config = LoadConfig("config.ini");
    int cellSize = config.cellSize;

    // setup Window
    InitWindow(config.screenWidth, config.screenHeight, "Advanced Particle Sim");
    SetTargetFPS(60);

    // instances
    Simulation simulation(config.screenWidth, config.screenHeight, cellSize);
    Renderer renderer(cellSize);
    UI ui(config.screenWidth, config.screenHeight);

    // variables for input
    ParticleType currentType = SAND;
    bool isCursorHidden = false;

    // game loop
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

        // draw
        BeginDrawing();
        ClearBackground(BLACK);

        renderer.Draw(simulation);
        currentType = ui.Draw(currentType);

        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}