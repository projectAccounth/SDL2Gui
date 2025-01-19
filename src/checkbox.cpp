#include "button.h"

CheckBox::CheckBox() :
    TextButton(),
    boxSymbol('X'),
    isChecked(false)
{}
CheckBox::CheckBox(
    TTF_Font* textFont,
    SDL_Color boxColor,
    SDL_Color textColor,
    std::optional<GuiObject*> parent,
    SDL_Renderer* renderer,
    UIUnit position, UIUnit size,
    char symbol
):
    TextButton(
        size,
        position,
        parent,
        renderer,
        boxColor,
        SDL_Color{
            (unsigned char)(boxColor.r - 122),
            (unsigned char)(boxColor.g - 122),
            (unsigned char)(boxColor.b - 122), 255
        },
        textColor,
        "",
        textFont
    ),
    boxSymbol(symbol),
    isChecked(false)
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

    if (!((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) && (active && visible))) {
        return;
    }
    if (x > objRect.x && x < (objRect.x + objRect.w) &&
        y > objRect.y && y < (objRect.y + objRect.h)) {
        hovered = true;
    }
    else {
        hovered = false;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && hovered) {
        isChecked = !isChecked;
        text = isChecked ? std::string(1, boxSymbol) : "";
        loadText(ref);
    }
}