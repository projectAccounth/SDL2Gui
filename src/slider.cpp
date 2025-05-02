#include "slider.h"
#include "guiobject.h"

using namespace GUILib::Reserved;

void GUILib::Slider::moveHandle(int delta)
{
    const SDL_Rect rect = getRect();

    switch (direction) {
        case DragDirection::HORIZONTAL:
            offsetX = static_cast<int>(clamp(offsetX + delta, 0, rect.w));
            break;
        case DragDirection::VERTICAL:
            offsetY = static_cast<int>(clamp(offsetY + delta, 0, rect.h));
            break;
    }
}

GUILib::Slider::Slider(
    std::shared_ptr<GuiObject> parent,
    SDL_Renderer* renderer,
    const UIUnit& size,
    const UIUnit& position,
    const SDL_Color& frameColor,
    const SDL_Color& handleColor
) :
	Frame(parent, renderer, size, position, frameColor),
    direction(DragDirection::HORIZONTAL),
    handleColor(handleColor),
    handleRect(),
    handleSize(15),
	offsetX(), offsetY(),
	lastMouseX(), lastMouseY(),
	draggingX(), draggingY()
{}

GUILib::Slider::Slider() = default;

void GUILib::Slider::render()
{
    shouldRenderChildren = false; // Doesn't render children (who would for a slider?)
    if (!shouldRender()) return;

    Frame::render();
    renderHandle();
}

void GUILib::Slider::renderHandle()
{
    const SDL_Rect rect = getRect();
    SDL_SetRenderDrawColor(ref, handleColor.r, handleColor.g, handleColor.b, handleColor.a);
    switch (direction) {
        case DragDirection::HORIZONTAL: {
            const int maxScrollX = rect.w;
            if (maxScrollX <= 0) break;
            const int scrollbarX = rect.x + (offsetX * (rect.w - handleSize)) / maxScrollX;
            handleRect = {scrollbarX, rect.y, handleSize, rect.h};
            SDL_SetRenderDrawColor(ref,
                handleColor.r,
                handleColor.r,
                handleColor.r,
                handleColor.a
            );
            SDL_RenderFillRect(ref, &handleRect);
            break;
        }

        case DragDirection::VERTICAL: {
            const int maxScrollY = rect.h;
            if (maxScrollY <= 0) break;
            const int scrollbarY = rect.y + (offsetY * (rect.h - handleSize)) / maxScrollY;
            handleRect = {rect.x, scrollbarY, rect.w, handleSize};
            SDL_SetRenderDrawColor(
                ref,
                handleColor.r,
                handleColor.r,
                handleColor.r,
                handleColor.a
            );
            SDL_RenderFillRect(ref, &handleRect);
            break;
        }
    }
}

void GUILib::Slider::handleEvent(const SDL_Event& event)
{
    GuiObject::handleEvent(event);
    if (!isActive()) return;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        const int mouseX = event.button.x;
        const int mouseY = event.button.y;

        if (direction == DragDirection::HORIZONTAL &&
            isPointInRect({ mouseX, mouseY }, handleRect)) {
            draggingX = true;
            lastMouseX = mouseX;
        }
        if (direction == DragDirection::VERTICAL &&
            isPointInRect({ mouseX, mouseY }, handleRect)) {
            draggingY = true;
            lastMouseY = mouseY;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        draggingX = draggingY = false;
    } else if (event.type == SDL_MOUSEMOTION) {
        if (draggingX) {
            const int deltaX = event.motion.x - lastMouseX;
            moveHandle(deltaX);
            lastMouseX = event.motion.x;
        }
        if (draggingY) {
            const int deltaY = event.motion.y - lastMouseY;
            moveHandle(deltaY);
            lastMouseY = event.motion.y;
        }
    }
}

int GUILib::Slider::getHandleSize() const
{
    return handleSize;
}

void GUILib::Slider::setHandleSize(int s)
{
    handleSize = s;
}

SDL_Color GUILib::Slider::getHandleColor() const
{
    return handleColor;
}

void GUILib::Slider::setHandleColor(SDL_Color color)
{
    handleColor = color;
}

GUILib::DragDirection GUILib::Slider::getDirection() const
{
    return direction;
}

void GUILib::Slider::setDirection(DragDirection dir)
{
    direction = dir;
}