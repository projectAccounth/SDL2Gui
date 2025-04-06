#include "slider.h"
#include "guiobject.h"

using namespace GUILib::Reserved;

void GUILib::Slider::moveHandle(int delta) {
    SDL_Rect rect = getRect();

    switch (direction) {
        case DragDirection::HORIZONTAL:
            offsetX = (int) clamp(offsetX + delta, 0, rect.w);
            break;
        case DragDirection::VERTICAL:
            offsetY = (int) clamp(offsetY + delta, 0, rect.h);
            break;
    }
}

void GUILib::Slider::updateHandlePosition()  {
    
}

GUILib::Slider::Slider(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    SDL_Color frameColor,
    SDL_Color handleColor) 
    : Frame(parent, renderer, size, position, frameColor),
    direction(DragDirection::HORIZONTAL),
    handleColor(handleColor),
    handleRect(),
    handleSize(15)
{
}

void GUILib::Slider::render() {
    shouldRenderChildren = false; // Doesn't render children
    if (!(isVisible() && isActive()) || !ref || (parent && !parent->isActive())) return;
    Frame::render();
    renderHandle();
}

void GUILib::Slider::renderHandle() {
    SDL_Rect rect = getRect();
    SDL_SetRenderDrawColor(ref, handleColor.r, handleColor.g, handleColor.b, handleColor.a);
    switch (direction) {
        case DragDirection::HORIZONTAL: {
            int maxScrollX = rect.w;
            if (maxScrollX <= 0) break;
            int scrollbarX = rect.x + (offsetX * (rect.w - handleSize)) / maxScrollX;
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
            int maxScrollY = rect.h;
            if (maxScrollY <= 0) break;
            int scrollbarY = rect.y + (offsetY * (rect.h - handleSize)) / maxScrollY;
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

void GUILib::Slider::handleEvent(const SDL_Event& event) {
    GuiObject::handleEvent(event);
    SDL_Rect rect = getRect();
    SDL_Point absContentSize = size.getAbsoluteSize({rect.w, rect.h});

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (direction == DragDirection::HORIZONTAL &&
            isPointInRect({ mouseX, mouseY }, handleRect)) {
            draggingX = true;
            lastMouseX = mouseX;
        }
        if (direction == DragDirection::VERTICAL &&
            isPointInRect({mouseX, mouseY}, handleRect)) {
            draggingY = true;
            lastMouseY = mouseY;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        draggingX = draggingY = false;
    } else if (event.type == SDL_MOUSEMOTION) {
        if (draggingX) {
            int deltaX = event.motion.x - lastMouseX;
            moveHandle(deltaX);
            lastMouseX = event.motion.x;
        }
        if (draggingY) {
            int deltaY = event.motion.y - lastMouseY;
            moveHandle(deltaY);
            lastMouseY = event.motion.y;
        }
    }
}

int GUILib::Slider::getHandleSize() const {
    return handleSize;
}

void GUILib::Slider::setHandleSize(int s) {
    handleSize = s;
}

SDL_Color GUILib::Slider::getHandleColor() const {
    return handleColor;
}

void GUILib::Slider::setHandleColor(SDL_Color color) {
    handleColor = color;
}

GUILib::DragDirection GUILib::Slider::getDirection() const {
    return direction;
}

void GUILib::Slider::setDirection(DragDirection dir) {
    direction = dir;
}