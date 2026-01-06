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
    SetTargetFPS(200);

    // instances
    Simulation simulation(config.screenWidth, config.screenHeight, cellSize);
    Renderer renderer(cellSize);
    UI ui(config.screenWidth, config.screenHeight);

    // variables for input
    ParticleType currentType = SAND;
    bool isCursorHidden = false;
    int brushRadius = 4;

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

            // if using ERASER, use the circular brush
            if (currentType == EMPTY) {
                simulation.SetParticleCircle(gridX, gridY, brushRadius, EMPTY);
            }
            else {
                // otherwise use single pixel (for sand, water, etc)
                simulation.SetParticle(gridX, gridY, currentType);
            }
        }

        simulation.Update();

        // draw
        BeginDrawing();
        ClearBackground(BLACK);

        renderer.Draw(simulation);

        // only draw if mouse is on the game field
        if (!ui.IsMouseOverUI()) {
            Vector2 mouse = GetMousePosition();
            // snap the drawing center to the grid center for visual accuracy
            int gridX = (int)mouse.x / cellSize;
            int gridY = (int)mouse.y / cellSize;
            float drawX = (gridX * cellSize) + (cellSize / 2.0f);
            float drawY = (gridY * cellSize) + (cellSize / 2.0f);

            // screen radius
            float drawRadius = brushRadius * cellSize;

            // red for eraser, White for others
            Color brushColor = (currentType == EMPTY) ? RED : WHITE;

            // draw transparent fill and solid outline
            DrawCircleV({ drawX, drawY }, drawRadius, Fade(brushColor, 0.2f));
            DrawCircleLinesV({ drawX, drawY }, drawRadius, Fade(brushColor, 0.5f));
        }

        currentType = ui.Draw(currentType);


        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}