#define _CRT_SECURE_NO_WARNINGS // raygui errors

#include "UI.h"
#include "raylib.h"


#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


UI::UI(int screenWidth, int screenHeight) {
    // menu dimensions
    float width = 140;
    float height = 300;
    float padding = 10;

    // calculate Top-Right position
    // x = Screen Width - Menu Width - Padding
    menuBounds = { (float)screenWidth - width - padding, padding, width, height };
}

bool UI::IsMouseOverUI() {
    Vector2 mouse = GetMousePosition();
    return (mouse.x >= menuBounds.x && mouse.x <= menuBounds.x + menuBounds.width &&
        mouse.y >= menuBounds.y && mouse.y <= menuBounds.y + menuBounds.height);
}

ParticleType UI::Draw(ParticleType currentType) {
    ParticleType selected = currentType;

    // can use RayGui styles here later to make it look cool
    // dor now, just a simple panel.

    // a semi-transparent background for the menu so we can see it
    DrawRectangleRec({ menuBounds.x, menuBounds.y, menuBounds.width, menuBounds.height }, Fade(BLACK, 0.5f));
    DrawRectangleLines((int)menuBounds.x, (int)menuBounds.y, (int)menuBounds.width, (int)menuBounds.height, WHITE);

    // starting coordinates for buttons
    float startX = menuBounds.x + 10;
    float startY = menuBounds.y + 10;
    float btnWidth = 120;
    float btnHeight = 30;
    float padding = 5;

    // helper Lambda to draw buttons easily
    auto DrawElementButton = [&](const char* text, ParticleType type, int index) {
        Rectangle btnRect = { startX, startY + (index * (btnHeight + padding)), btnWidth, btnHeight };

        // fraw the Button first
        bool clicked = GuiButton(btnRect, text);

        // if this is the ACTIVE tool, draw a thick Green outline ON TOP
        if (currentType == type) {
            DrawRectangleLinesEx(btnRect, 3, GREEN);
        }

        // return result
        if (clicked) {
            return type;
        }
        return selected;
        };

    // draw The Buttons
    selected = DrawElementButton("SAND", SAND, 0);
    selected = DrawElementButton("STONE", STONE, 1);
    selected = DrawElementButton("WATER", WATER, 2);
    selected = DrawElementButton("SMOKE", SMOKE, 3);

    return selected;
}