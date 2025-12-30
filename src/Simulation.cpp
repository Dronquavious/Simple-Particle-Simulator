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

  // update from bottom up
  for (int y = rows - 2; y >= 0; y--) {
    for (int x = 0; x < cols; x++) {
      if (!moved[y][x]) {
        switch (grid[y][x]) {
          case SAND:
            MoveSand(x, y);
            break;
          case WATER:
            MoveWater(x, y);
            break;
          case SMOKE:
            MoveSmoke(x, y);
            break;
          case FIRE:
            MoveFire(x, y);
            break;
          default:
            break;
        }
      }
    }
  }
}

void Simulation::MoveSand(int x, int y) {
  // try down
  if (y + 1 < rows && grid[y + 1][x] == EMPTY) {
    grid[y][x] = EMPTY;
    grid[y + 1][x] = SAND;
    moved[y][x] = true;
    moved[y + 1][x] = true;
    return;
  }

  int dir = GetRandomValue(0, 1) ? 1 : -1;

  // try diagonals (random order)
  int dx[2] = {dir, -dir};
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    int ny = y + 1;
    if (IsWithinBounds(nx, ny) && grid[ny][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[ny][nx] = SAND;
      moved[y][x] = true;
      moved[ny][nx] = true;
      return;
    }
  }

  moved[y][x] = true;
}

void Simulation::MoveWater(int x, int y) {
  // down
  if (y + 1 < rows && grid[y + 1][x] == EMPTY) {
    grid[y][x] = EMPTY;
    grid[y + 1][x] = WATER;
    moved[y][x] = true;
    moved[y + 1][x] = true;
    return;
  }

  int dir = GetRandomValue(0, 1) ? 1 : -1;
  int dx[2] = {dir, -dir};

  // diagonals
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    int ny = y + 1;
    if (IsWithinBounds(nx, ny) && grid[ny][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[ny][nx] = WATER;
      moved[y][x] = true;
      moved[ny][nx] = true;
      return;
    }
  }

  // horizontal spread
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    if (IsWithinBounds(nx, y) && grid[y][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[y][nx] = WATER;
      moved[y][x] = true;
      moved[y][nx] = true;
      return;
    }
  }

  moved[y][x] = true;
}

// moves just like water but upwards
void Simulation::MoveSmoke(int x, int y) {
  // dissipate
  if (GetRandomValue(0, 500) == 0) {
    grid[y][x] = EMPTY;
    moved[y][x] = true;
    return;
  }

  // float / flicker
  if (GetRandomValue(0, 3) == 0) {
    moved[y][x] = true;
    return;
  }

  int dir = GetRandomValue(0, 1) ? 1 : -1;
  int dx[2] = {dir, -dir};

  // up
  if (y > 0 && grid[y - 1][x] == EMPTY) {
    grid[y][x] = EMPTY;
    grid[y - 1][x] = SMOKE;
    moved[y][x] = true;
    moved[y - 1][x] = true;
    return;
  }

  // diagonals up
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    int ny = y - 1;
    if (IsWithinBounds(nx, ny) && grid[ny][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[ny][nx] = SMOKE;
      moved[y][x] = true;
      moved[ny][nx] = true;
      return;
    }
  }

  // horizontal drift
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    if (IsWithinBounds(nx, y) && grid[y][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[y][nx] = SMOKE;
      moved[y][x] = true;
      moved[y][nx] = true;
      return;
    }
  }

  moved[y][x] = true;
}

void Simulation::MoveFire(int x, int y) {
  // ignite nearby flammable materials
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      int nx = x + dx;
      int ny = y + dy;
      if (IsWithinBounds(nx, ny) && grid[ny][nx] == WOOD) {
        if (GetRandomValue(0, 20) == 0) {
          grid[ny][nx] = FIRE;
          moved[ny][nx] = true;
        }
      }
    }
  }

  // chance to extinguish
  if (GetRandomValue(0, 500) == 0) {
    grid[y][x] = EMPTY;
    moved[y][x] = true;
    return;
  }

  // chance to turn into smoke
  if (GetRandomValue(0, 10) == 0) {
    grid[y][x] = SMOKE;
    moved[y][x] = true;
    return;
  }

  // chance to flicker (stay in place)
  if (GetRandomValue(0, 4) == 0) {
    moved[y][x] = true;
    return;
  }

  int dir = GetRandomValue(0, 1) ? 1 : -1;

  // try up
  if (y > 0 && grid[y - 1][x] == EMPTY) {
    grid[y][x] = EMPTY;
    grid[y - 1][x] = FIRE;
    moved[y][x] = true;
    moved[y - 1][x] = true;
    return;
  }

  // try diagonals (randomized)
  int dx[2] = {dir, -dir};
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    int ny = y - 1;
    if (IsWithinBounds(nx, ny) && grid[ny][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[ny][nx] = FIRE;
      moved[y][x] = true;
      moved[ny][nx] = true;
      return;
    }
  }

  // horizontal drift
  for (int i = 0; i < 2; i++) {
    int nx = x + dx[i];
    if (IsWithinBounds(nx, y) && grid[y][nx] == EMPTY) {
      grid[y][x] = EMPTY;
      grid[y][nx] = FIRE;
      moved[y][x] = true;
      moved[y][nx] = true;
      return;
    }
  }

  moved[y][x] = true;
}
