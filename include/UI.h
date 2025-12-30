#pragma once
#include "Simulation.h" 

class UI {
public:
    // requires screen size to position itself
    UI(int screenWidth, int screenHeight);

    ParticleType Draw(ParticleType currentType);
    bool IsMouseOverUI();

private:
    struct {
        float x, y, width, height;
    } menuBounds;
};