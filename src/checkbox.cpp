#include "button.h"

GUILib::CheckBox::CheckBox():
    boxSymbol('X'),
    checked(false)
{}
GUILib::CheckBox::CheckBox(
    std::shared_ptr<GuiObject> parent,
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
            static_cast<uint8_t>(boxColor.r - 122),
            static_cast<uint8_t>(boxColor.g - 122),
            static_cast<uint8_t>(boxColor.b - 122), 255
        },
        textColor,
        "",
        textFont
    ),
    boxSymbol(symbol),
    checked(false)
{}

void GUILib::CheckBox::changeSymbol(const char& symbol) {
    boxSymbol = symbol;
    if (isChecked())
        text = boxSymbol;
}

void GUILib::CheckBox::handleEvent(const SDL_Event& e) {
    if (!active || !visible) return;

    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    // Suppress this warning please, I didn't mean to call the direct superclass
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

GUILib::CheckBox::CheckBox(const CheckBox& other):
	boxSymbol(other.boxSymbol), checked(other.checked)
{
	
}

GUILib::CheckBox& GUILib::CheckBox::operator=(const CheckBox& other)
{
    if (this == &other) return *this;

    boxSymbol = other.boxSymbol;
    checked = other.checked;

    return *this;
}
