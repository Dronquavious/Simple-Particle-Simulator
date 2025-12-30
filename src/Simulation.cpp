#include "Simulation.h"
#include "raylib.h"

Simulation::Simulation(int width, int height, int cellSize) {
    // calculate grid dimensions
    cols = width / cellSize;
    rows = height / cellSize;

    // initialize the vectors
    grid.resize(rows, std::vector<ParticleType>(cols, EMPTY));
    moved.resize(rows, std::vector<bool>(cols, false));
}

void Simulation::SetParticle(int x, int y, ParticleType type) {
    if (IsWithinBounds(x, y)) {
        grid[y][x] = type;
    }
}

const std::vector<std::vector<ParticleType>>& Simulation::GetGrid() const {
    return grid;
}

bool Simulation::IsWithinBounds(int x, int y) {
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

void Simulation::Update() {
    // reset moved status
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            moved[y][x] = false;
        }
    }

    // update from bottom up (standard falling sand logic)
    for (int y = rows - 2; y >= 0; y--) {
        for (int x = 0; x < cols; x++) {
            if (!moved[y][x]) {
                if (grid[y][x] == SAND) {
                    MoveSand(x, y);
                }
                else if (grid[y][x] == WATER) {
                    MoveWater(x, y);
                }
                else if (grid[y][x] == SMOKE) {
                    MoveSmoke(x, y);
                }
            }
        }
    }
}

void Simulation::MoveSand(int x, int y) {
    // try to move down
    if (y + 1 < rows && grid[y + 1][x] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x] = SAND;
        moved[y + 1][x] = true;
        return;
    }

    // try to move diagonally
    int direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    // check one random diagonal
    if (IsWithinBounds(x + direction, y + 1) && grid[y + 1][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x + direction] = SAND;
        moved[y + 1][x + direction] = true;
    }
    // check the other diagonal
    else if (IsWithinBounds(x - direction, y + 1) && grid[y + 1][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x - direction] = SAND;
        moved[y + 1][x - direction] = true;
    }
}

void Simulation::MoveWater(int x, int y) {
    // try to move down
    if (y + 1 < rows && grid[y + 1][x] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x] = WATER;
        moved[y + 1][x] = true;
        return;
    }

    // try to move diagonally
    int direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (IsWithinBounds(x + direction, y + 1) && grid[y + 1][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x + direction] = WATER;
        moved[y + 1][x + direction] = true;
        return;
    }
    else if (IsWithinBounds(x - direction, y + 1) && grid[y + 1][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y + 1][x - direction] = WATER;
        moved[y + 1][x - direction] = true;
        return;
    }

    // try to move horizontally (dispersing like liquid)
    direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (IsWithinBounds(x + direction, y) && grid[y][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y][x + direction] = WATER;
        moved[y][x + direction] = true;
    }
    else if (IsWithinBounds(x - direction, y) && grid[y][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y][x - direction] = WATER;
        moved[y][x - direction] = true;
    }
}

// moves just like water but upwards
void Simulation::MoveSmoke(int x, int y) {

    // random chance to dissapear to make it look more natural
    if (GetRandomValue(0, 500) < 1) {
        grid[y][x] = EMPTY;
        return;
    }
    
    // random chance to stay still (makes it look lighter and "floaty")
    if (GetRandomValue(0, 10) < 5) {
        return;
    }

    // try to move up
    if (y - 1 >= 0 && grid[y - 1][x] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y - 1][x] = SMOKE;
        moved[y - 1][x] = true;
        return;
    }

    // try to move diagonally
    int direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (IsWithinBounds(x + direction, y - 1) && grid[y - 1][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y - 1][x + direction] = SMOKE;
        moved[y - 1][x + direction] = true;
        return;
    }
    else if (IsWithinBounds(x - direction, y - 1) && grid[y - 1][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y - 1][x - direction] = SMOKE;
        moved[y - 1][x - direction] = true;
        return;
    }

    // try to move horizontally
    direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    if (IsWithinBounds(x + direction, y) && grid[y][x + direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y][x + direction] = SMOKE;
        moved[y][x + direction] = true;
    }
    else if (IsWithinBounds(x - direction, y) && grid[y][x - direction] == EMPTY) {
        grid[y][x] = EMPTY;
        grid[y][x - direction] = SMOKE;
        moved[y][x - direction] = true;
    }
}