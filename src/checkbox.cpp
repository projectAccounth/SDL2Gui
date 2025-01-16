#include "button.h"

CheckBox::CheckBox() {}
CheckBox::CheckBox(
    TTF_Font* textFont,
    int x = 0, int y = 0, int w = 10, int h = 10,
    SDL_Color boxColor,
    SDL_Color textColor,
    char symbol
):
    TextButton(
        x, y, w, h,
        boxColor,
        "",
        textColor,
        SDL_Color{ 
            (unsigned char)(boxColor.r - 122),
            (unsigned char)(boxColor.g - 122),
            (unsigned char)(boxColor.b - 122), 255 
        },
        textFont
    )
{}

void CheckBox::changeSymbol(char symbol) {
    boxSymbol = symbol;
    if (isChecked)
        text = boxSymbol;
}

void CheckBox::handleEvents(SDL_Event& e) {
    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    if ((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) && (active && visible)) {
        if (x > objRect.x && x < (objRect.x + objRect.w) &&
            y > objRect.y && y < (objRect.y + objRect.h)) {
            hovered = true;
            if (hoverAction) hoverAction();
        }
        else {
            hovered = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN && hovered) {
            if (buttonAction) buttonAction(); // optional
            isChecked = !isChecked;
            text = isChecked ? std::string(1, boxSymbol) : "";
        }
    }
}