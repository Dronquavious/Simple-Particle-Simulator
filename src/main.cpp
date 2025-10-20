#include "raylib.h"
#include <vector>

enum ParticleType
{
    EMPTY = 0,
    SAND = 1,
    STONE = 2,
    WATER = 3,
};

void moveSand(int x, int y, std::vector<std::vector<ParticleType>>& grid, std::vector<std::vector<bool>>& moved, int rows, int collums) {
    // Try to move down
    if (y + 1 < rows && grid[y + 1][x] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x] = SAND;
        moved[y + 1][x] = true;
        return;
    }

    // Try to move diagonally
    int direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (x + direction >= 0 && x + direction < collums && y + 1 < rows && grid[y + 1][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x + direction] = SAND;
        moved[y + 1][x + direction] = true;
    } else if (x - direction >= 0 && x - direction < collums && y + 1 < rows && grid[y + 1][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x - direction] = SAND;
        moved[y + 1][x - direction] = true;
    }
}

void moveWater(int x, int y, std::vector<std::vector<ParticleType>>& grid, std::vector<std::vector<bool>>& moved, int rows, int collums) {
    // Try to move down
    if (y + 1 < rows && grid[y + 1][x] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x] = WATER;
        moved[y + 1][x] = true;
        return;
    }

    // Try to move diagonally
    int direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (x + direction >= 0 && x + direction < collums && y + 1 < rows && grid[y + 1][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x + direction] = WATER;
        moved[y + 1][x + direction] = true;
        return;
    } else if (x - direction >= 0 && x - direction < collums && y + 1 < rows && grid[y + 1][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x - direction] = WATER;
        moved[y + 1][x - direction] = true;
        return;
    }

    // Try to move horizontally
    direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (x + direction >= 0 && x + direction < collums && grid[y][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y][x + direction] = WATER;
        moved[y][x + direction] = true;
    } else if (x - direction >= 0 && x - direction < collums && grid[y][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y][x - direction] = WATER;
        moved[y][x - direction] = true;
    }
}

void updateParticles(std::vector<std::vector<ParticleType>>& grid, std::vector<std::vector<bool>>& moved, int rows, int collums) {
    for (int y = rows - 2; y >= 0; y--) {
        for (int x = 0; x < collums; x++) {
            if (!moved[y][x]) {
                if (grid[y][x] == SAND) {
                    moveSand(x, y, grid, moved, rows, collums);
                } else if (grid[y][x] == WATER) {
                    moveWater(x, y, grid, moved, rows, collums);
                }
            }
        }
    }
}

int main(void)
{

    const int screenWidth = 1280;
    const int screenHeight = 720;

    // --------------------------------------------------------------------------------------------------------
    // 2d grid to represent where sand is falling
    // EMPTY represent empty, SAND represents sand, STONE represent stone, WATER water

    ParticleType selectedParticle = SAND;

    int cellSize = 20;
    int rows = screenHeight / cellSize;
    int collums = screenWidth / cellSize;
    
    std::vector<std::vector<ParticleType>> grid(rows, std::vector<ParticleType>(collums, EMPTY));
    std::vector<std::vector<bool>> moved(rows, std::vector<bool>(collums, false));

    InitWindow(screenWidth, screenHeight, "sand particle simulator");

    bool isCursorHidden = false;

    SetTargetFPS(60);

    // main game loop
    while (!WindowShouldClose())
    {

        // reset the moved grid at the start of each frame
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < collums; x++)
            {
                moved[y][x] = false;
            }
        }

        int gridX = GetMouseX() / cellSize;
        int gridY = GetMouseY() / cellSize;

        // making cursor hidden
        if (IsKeyPressed(KEY_H))
        {
            isCursorHidden = !isCursorHidden;
            if (isCursorHidden)
            {
                HideCursor();
            }
            else
            {
                ShowCursor();
            }
        }

        // mode changing
        if (IsKeyPressed(KEY_E))
        {
            selectedParticle = WATER;
        }

        if (IsKeyPressed(KEY_Q))
        {
            selectedParticle = SAND;
        }

        if (IsKeyPressed(KEY_W))
        {
            selectedParticle = STONE;
        }

        // Update the mode handling
        if (selectedParticle != EMPTY)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                if (gridY >= 0 && gridY < rows && gridX >= 0 && gridX < collums)
                {
                    grid[gridY][gridX] = selectedParticle;
                }
            }
        }

        BeginDrawing();

        ClearBackground(BROWN);

        updateParticles(grid, moved, rows, collums);

        // drawing whether sand or stone or water
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < collums; x++)
            {
                if (grid[y][x] == SAND)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, YELLOW);
                }
                else if (grid[y][x] == STONE)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, GRAY);
                }
                else if (grid[y][x] == WATER)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLUE);
                }
            }
        }

        if (isCursorHidden)
        {
            DrawText("CURSOR HIDDEN", 20, 10, 20, RED);
        }
        else
        {
            DrawText("CURSOR VISIBLE", 20, 10, 20, LIME);
        }

        DrawText("Press 'Q' for Sand, 'W' for Stone, 'E' for Water", 20, 30, 15, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
