#include "raylib.h"
#include <vector>

enum ParticleType
{
    EMPTY = 0,
    SAND = 1,
    STONE = 2,
    WATER = 3,
};

int main(void)
{

    const int screenWidth = 1280;
    const int screenHeight = 720;

    // --------------------------------------------------------------------------------------------------------
    // 2d grid to represent where sand is falling
    // EMPTY represent empty, SAND represents sand, STONE represent stone, WATER water

    ParticleType selectedParticle = SAND;

    int cellSize = 20;
    int rows = screenHeight / cellSize;    // rows based on screen height shuld be 72
    int collums = screenWidth / cellSize;  // columns based on screen width shuld be 128
    
    std::vector<std::vector<ParticleType>> grid(rows, std::vector<ParticleType>(collums, EMPTY)); // empty grid
    std::vector<std::vector<bool>> moved(rows, std::vector<bool>(collums, false)); // used to track if a cell has been moved this frame

    

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

        int gridX = GetMouseX() / cellSize; // x pixel to a grid collum same for y
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
                    grid[gridY][gridX] = selectedParticle; // placing sand, stone, or water
                }
            }
        }

        BeginDrawing();

        ClearBackground(BROWN);

        // loop from bottom to top for so particle falls correctly  
        for (int y = rows - 2; y >= 0; y--)
        {
            for (int x = 0; x < collums; x++)
            {
                // check if current cell is sand or water and hasn't moved this frame
                if ((grid[y][x] == SAND || grid[y][x] == WATER) && !moved[y][x])
                {
                    // check if the cell below is empty
                    if (y + 1 < rows && grid[y + 1][x] == EMPTY)
                    {
                        // move particle down
                        grid[y + 1][x] = grid[y][x];
                        grid[y][x] = EMPTY;
                        moved[y + 1][x] = true;
                    }
                    else
                    {
                        // randomly choose left or right direction
                        int randomDirection = GetRandomValue(0, 1);
                        
                        // try both left and right directions
                        for (int i = 0; i < 2; i++)
                        {
                            // calculate horizontal direction (-1 for left, 1 for right)
                            int dx = (randomDirection + i) % 2 == 0 ? -1 : 1;

                            // check if new position is within grid bounds
                            if (x + dx >= 0 && x + dx < collums && y + 1 < rows)
                            {
                                // handle sand movement
                                if (grid[y][x] == SAND)
                                {
                                    // check if diagonal cell is empty
                                    if (grid[y + 1][x + dx] == EMPTY)
                                    {
                                        // move sand diagonally
                                        grid[y + 1][x + dx] = SAND;
                                        grid[y][x] = EMPTY;
                                        moved[y + 1][x + dx] = true;
                                        break;
                                    }
                                }
                                // handle water movement
                                else if (grid[y][x] == WATER)
                                {
                                    bool hasMovedWater = false;

                                    // try to move straight down
                                    if (grid[y + 1][x] == EMPTY)
                                    {
                                        grid[y + 1][x] = WATER;
                                        grid[y][x] = EMPTY;
                                        moved[y + 1][x] = true;
                                        hasMovedWater = true;
                                    }
                                    else
                                    {
                                        // if moving down doesnt work try diagonal movements
                                        int directions[] = {-1, 1};
                                        for (int j = 0; j < 2 && !hasMovedWater; j++)
                                        {
                                            int dx_water = directions[GetRandomValue(0, 1)];
                                            if (x + dx_water >= 0 && x + dx_water < collums && grid[y + 1][x + dx_water] == EMPTY)
                                            {
                                                grid[y + 1][x + dx_water] = WATER;
                                                grid[y][x] = EMPTY;
                                                moved[y + 1][x + dx_water] = true;
                                                hasMovedWater = true;
                                            }
                                        }

                                        // if still hasnt moved try horizontal movement
                                        if (!hasMovedWater)
                                        {
                                            for (int j = 0; j < 2 && !hasMovedWater; j++)
                                            {
                                                int dx_water = directions[GetRandomValue(0, 1)];
                                                if (x + dx_water >= 0 && x + dx_water < collums && grid[y][x + dx_water] == EMPTY)
                                                {
                                                    grid[y][x + dx_water] = WATER;
                                                    grid[y][x] = EMPTY;
                                                    moved[y][x + dx_water] = true;
                                                    hasMovedWater = true;
                                                }
                                            }
                                        }
                                    }
                                    if (hasMovedWater)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // drawing whether sand or stone or water
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < collums; x++)
            {
                if (grid[y][x] == SAND)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, YELLOW); // Sand
                }
                else if (grid[y][x] == STONE)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, GRAY); // Stone
                }
                else if (grid[y][x] == WATER)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLUE); // Water
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