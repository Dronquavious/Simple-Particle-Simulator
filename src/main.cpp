
#include "raylib.h"
#include <iostream>

int main(void)
{

    const int screenWidth = 1280;
    const int screenHeight = 720;

    // --------------------------------------------------------------------------------------------------------
    // 2d grid to represent where sand is falling
    // 0 represent empty, 1 represents sand, 2 represent stone, 3 water

    int mode = 0; // 0: none, 1: sand, 2: stone, 3: water

    int cellSize = 10;
    int rows = screenHeight / cellSize;    // rows based on screen height shuld be 72
    int collums = screenWidth / cellSize;  // columns based on screen width shuld be 128
    
    std::vector<std::vector<int>> grid(rows, std::vector<int>(collums, 0)); // empty grid
    std::vector<std::vector<bool>> moved(rows, std::vector<bool>(collums, false)); // used to track if a cell has been moved this frame

    

    InitWindow(screenWidth, screenHeight, "sand particle simulator");

    int isCursorHidden = 0;

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
            if (isCursorHidden == 0)
            {
                HideCursor();
                isCursorHidden = 1;
            }
            else
            {
                ShowCursor();
                isCursorHidden = 0;
            }
        }

        // mode changing
        if (IsKeyPressed(KEY_E))
        {
            mode = 3;
        }

        if (IsKeyPressed(KEY_Q))
        {
            mode = 1;
        }

        if (IsKeyPressed(KEY_W))
        {
            mode = 2;
        }

        // Update the mode handling
        if (mode == 1 || mode == 2 || mode == 3)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                grid[gridY][gridX] = mode; // placing sand, stone, or water
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
                if ((grid[y][x] == 1 || grid[y][x] == 3) && !moved[y][x])
                {
                    // check if the cell below is empty
                    if (grid[y + 1][x] == 0)
                    {
                        // move particle down
                        grid[y + 1][x] = grid[y][x];
                        grid[y][x] = 0;
                        moved[y + 1][x] = true;
                    }
                    else
                    {
                        // randomly choose left or right direction
                        int randomDirection = GetRandomValue(0, 1);
                        bool hasMoved = false;

                        // try both left and right directions
                        for (int i = 0; i < 2; i++)
                        {
                            // calculate horizontal direction (-1 for left, 1 for right)
                            int dx = (randomDirection + i) % 2 == 0 ? -1 : 1;

                            // check if new position is within grid bounds
                            if (x + dx >= 0 && x + dx < collums)
                            {
                                // handle sand movement
                                if (grid[y][x] == 1)
                                {
                                    // check if diagonal cell is empty
                                    if (grid[y + 1][x + dx] == 0)
                                    {
                                        // move sand diagonally
                                        grid[y + 1][x + dx] = 1;
                                        grid[y][x] = 0;
                                        moved[y + 1][x + dx] = true;
                                        hasMoved = true;
                                        break;
                                    }
                                }
                                // handle water movement
                                else if (grid[y][x] == 3)
                                {
                                    bool hasMoved = false;

                                    // try to move straight down
                                    if (grid[y + 1][x] == 0)
                                    {
                                        grid[y + 1][x] = 3;
                                        grid[y][x] = 0;
                                        moved[y + 1][x] = true;
                                        hasMoved = true;
                                    }
                                    else
                                    {
                                        // if moving down doesnt work try diagonal movements
                                        int directions[] = {-1, 1};
                                        for (int i = 0; i < 2 && !hasMoved; i++)
                                        {
                                            int dx = directions[GetRandomValue(0, 1)];
                                            if (x + dx >= 0 && x + dx < collums && grid[y + 1][x + dx] == 0)
                                            {
                                                grid[y + 1][x + dx] = 3;
                                                grid[y][x] = 0;
                                                moved[y + 1][x + dx] = true;
                                                hasMoved = true;
                                            }
                                        }

                                        // if still hasnt moved try horizontal movement
                                        if (!hasMoved)
                                        {
                                            for (int i = 0; i < 2 && !hasMoved; i++)
                                            {
                                                int dx = directions[GetRandomValue(0, 1)];
                                                if (x + dx >= 0 && x + dx < collums && grid[y][x + dx] == 0)
                                                {
                                                    grid[y][x + dx] = 3;
                                                    grid[y][x] = 0;
                                                    moved[y][x + dx] = true;
                                                    hasMoved = true;
                                                }
                                            }
                                        }
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
                if (grid[y][x] == 1)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, YELLOW); // Sand
                }
                else if (grid[y][x] == 2)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, GRAY); // Stone
                }
                else if (grid[y][x] == 3)
                {
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLUE); // Water
                }
            }
        }

        if (isCursorHidden == 1)
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
