#include "button.h"

GUILib::CheckBox::CheckBox() :
    TextButton(),
    boxSymbol('X'),
    checked(false)
{}
GUILib::CheckBox::CheckBox(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    TTF_Font* textFont,
    SDL_Color boxColor,
    SDL_Color textColor,
    char symbol
):
    TextButton(
        parent, renderer,
        size, position,
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
    boxSymbol(std::move(symbol)),
    checked(false)
{}

void GUILib::CheckBox::changeSymbol(const char& symbol) {
    boxSymbol = symbol;
    if (isChecked())
        text = boxSymbol;
}

void GUILib::CheckBox::handleEvent(const SDL_Event& e) {
    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    GuiObject::handleEvent(e);

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
        toggleChecked();
        text = isChecked() ? std::string(1, boxSymbol) : "";
        initialize(ref);
        trigger("onClick");
    }
}

void GUILib::CheckBox::toggleChecked() { checked = !checked; }
void GUILib::CheckBox::toggleChecked(bool val) { checked = val; }
bool GUILib::CheckBox::isChecked() const { return checked; }
char GUILib::CheckBox::getSymbol() const { return boxSymbol; }