#pragma once
#include "Simulation.h" // grid needs to be read

class Renderer {
public:
    Renderer(int cellSize);

    void Draw(const Simulation& simulation);

private:
    int cellSize;
};