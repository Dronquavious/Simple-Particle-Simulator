#include "Renderer.h"
#include "raylib.h"

Renderer::Renderer(int cellSize)
    : cellSize(cellSize) {
}

void Renderer::Draw(const Simulation& simulation) {
    // get the data from the simulation
    const auto& grid = simulation.GetGrid();
    int rows = simulation.GetRows();
    int cols = simulation.GetCols();

    // loop and Draw
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {

            ParticleType type = grid[y][x];

            if (type != EMPTY) {
                Color color = BLACK; // sefault

                // color Palette
                // will upgrade this later to be more "visual" (varying shades)
                switch (type) {
                case SAND:  color = YELLOW; break;
                case STONE: color = DARKGRAY; break;
                case WATER: color = BLUE; break;
                case SMOKE: color = LIGHTGRAY; break;
                default:    color = MAGENTA; break; // error color
                }

                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
            }
        }
    }
}