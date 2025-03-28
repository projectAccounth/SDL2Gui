#include "frame.h"

GUILib::Frame::Frame() : GuiObject(), frameColor(SDL_Color())
{
}
GUILib::Frame::Frame(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    SDL_Color frameColor,
    bool isVisible, bool isActive) : GuiObject(parent, renderer, size, position, isVisible, isActive),
                                     frameColor(frameColor)
{
}
void GUILib::Frame::render() {
    if (!(isVisible()) || !ref || (parent && !parent->isVisible())) return;
    GuiObject::render(); // super.render()

    SDL_SetRenderDrawColor(ref, frameColor.r, frameColor.g, frameColor.b, frameColor.a);
    SDL_RenderFillRect(ref, &objRect);
}

void GUILib::Frame::setFrameColor(const SDL_Color& color) { frameColor = color; }
SDL_Color GUILib::Frame::getFrameColor() const { return frameColor; }