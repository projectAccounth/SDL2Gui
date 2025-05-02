#include "button.h"

using namespace GUILib;

Button::Button():
    hovered(false)
{}
Button::Button(
    std::shared_ptr<GuiObject> parent,
    SDL_Renderer* renderer,
    UIUnit size,
    UIUnit position
):
    GuiObject(parent, renderer, size, position),
    hovered(false)
{
}

bool Button::isClicked(int x, int y) const
{
    return (x > objRect.x &&
        x < (objRect.x + objRect.w) &&
        y > objRect.y &&
        y < (objRect.y + objRect.h));
}

void Button::checkHover(int mouseX, int mouseY)
{
    hovered = isClicked(mouseX, mouseY);
}

void Button::handleEvent(const SDL_Event& e)
{
    if (!active || !visible) return;

    GuiObject::handleEvent(e);
    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    if (!((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) && active && visible)) {
        return;
    }
    if (x > objRect.x && x < (objRect.x + objRect.w) &&
        y > objRect.y && y < (objRect.y + objRect.h)) {
        hovered = true;
        if (hoverAction) hoverAction();
        trigger("onHover", x, y);
    }
    else {
        hovered = false;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && hovered &&
        e.button.button == SDL_BUTTON_LEFT) {
        if (buttonAction) buttonAction();
        trigger("onClick", x, y);
    }
}

void Button::setHoverAction(const std::function<void()>& actionFunction)
{
    buttonAction = actionFunction;
}

void Button::setAction(const std::function<void()>& actionFunction)
{
    buttonAction = actionFunction;
}

bool Button::isHovered() const
{
    return hovered;
}

Button& Button::operator=(const Button& other)
{
    // Seriously, nothing is worth copying
    GuiObject::operator=(other);

    return *this;
}

Button& Button::operator=(Button&& other) noexcept
{
    GuiObject::operator=(other);
    return *this;
}