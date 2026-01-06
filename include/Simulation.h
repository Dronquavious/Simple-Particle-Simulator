#pragma once
#include <vector>


enum ParticleType {
    EMPTY = 0,
    SAND = 1,
    STONE = 2,
    WATER = 3,
    SMOKE = 4,
    FIRE = 5,
    WOOD = 6,
    ACID = 7,
};

class Simulation {
public:
    // sets up the grid size
    Simulation(int width, int height, int cellSize);

    // main physics step (formerly updateParticles)
    void Update();

    // Helper to set a particle (used by mouse input)
    void SetParticle(int gridX, int gridY, ParticleType type);

    // getter so the Renderer knows what to draw
    const std::vector<std::vector<ParticleType>>& GetGrid() const;

    // getters for dimensions
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }

private:
    int rows;
    int cols;

    // the data grids
    std::vector<std::vector<ParticleType>> grid;
    std::vector<std::vector<bool>> moved;

    // internal physics helpers (Private because only Update() uses them)
    void MoveSand(int x, int y);
    void MoveWater(int x, int y);
    void MoveSmoke(int x, int y);
    void MoveFire(int x, int y);
    void MoveAcid(int x, int y);
    bool IsWithinBounds(int x, int y);
};