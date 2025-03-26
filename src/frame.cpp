#include "frame.h"

GUILib::Frame::Frame() : GuiObject(), frameColor(SDL_Color()) {}
GUILib::Frame::Frame(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    SDL_Color frameColor,
    bool isVisible, bool isActive) : GuiObject(parent, renderer, size, position, isVisible, isActive),
                                     frameColor(frameColor) {}

void GUILib::Frame::render() {
    if (!(visible && active)) return;
    GuiObject::render(); // super.render()

    SDL_SetRenderDrawColor(ref, frameColor.r, frameColor.g, frameColor.b, frameColor.a);
    SDL_RenderFillRect(ref, &objRect);
}

void GUILib::Frame::setFrameColor(const SDL_Color& color) { frameColor = color; }
SDL_Color GUILib::Frame::getFrameColor() const { return frameColor; }

GUILib::ScrollingFrame::ScrollingFrame(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    UIUnit contentSize,
    SDL_Color frameColor,
    bool isVisible,
    bool isActive
) :
    Frame(parent, renderer, size, position, frameColor, isVisible, isActive),
    contentSize(contentSize),
    scrollX(0),
    scrollY(0),
    showScrollbarX(false),
    showScrollbarY(false),
    scrollingBarColor() {}

void GUILib::ScrollingFrame::setContentSize(const UIUnit& newSize) {
    if (newSize.isUsingScale == size.isUsingScale) {
        contentSize.sizeX = std::max(newSize.sizeX, size.sizeX);
        contentSize.sizeY = std::max(newSize.sizeY, size.sizeY);
    }
    else if (newSize.isUsingScale) {
        contentSize.sizeX = std::max(newSize.sizeX * size.sizeX, size.sizeX);
        contentSize.sizeY = std::max(newSize.sizeY * size.sizeY, size.sizeY);
    }
    else {
        contentSize.sizeX = std::max(newSize.sizeX, size.sizeX * newSize.sizeX);
        contentSize.sizeY = std::max(newSize.sizeY, size.sizeY * newSize.sizeY);
    }
    // Holy shit
    contentSize.isUsingScale = size.isUsingScale;
    updateScrollbars();
}

void GUILib::ScrollingFrame::render() {
    Frame::render();  // Render base frame

    SDL_Rect clipRect = getRect();
    SDL_RenderSetClipRect(ref, &clipRect);

    // Convert contentSize to absolute size
    SDL_Point absContentSize = contentSize.getAbsoluteSize({ clipRect.w, clipRect.h });

    // Render children inside the scrolling region
    for (auto& child : children) {
        SDL_Rect childRect = child->getRect();
        childRect.x -= scrollX;
        childRect.y -= scrollY;

        if (SDL_HasIntersection(&clipRect, &childRect)) {
            child->render();
        }
    }

    SDL_RenderSetClipRect(ref, nullptr);

    updateScrollbars();

    renderScrollbars(absContentSize, clipRect);
}

void GUILib::ScrollingFrame::renderScrollbars(const SDL_Point& absContentSize, const SDL_Rect& rect) {
    SDL_SetRenderDrawColor(ref,
        scrollingBarColor.r,
        scrollingBarColor.g,
        scrollingBarColor.b,
        scrollingBarColor.a
    );  // Dark gray scrollbars (by default)
    if (showScrollbarX) {
        int maxScrollX = absContentSize.x - rect.w;
        if (maxScrollX > 0) {
            int scrollbarWidth = std::max(20, (rect.w * rect.w) / absContentSize.x);
            int scrollbarX = rect.x + (scrollX * (rect.w - scrollbarWidth)) / maxScrollX;

            SDL_Rect scrollbarRect = { scrollbarX, rect.y + rect.h - 8, scrollbarWidth, 8 };
            SDL_SetRenderDrawColor(ref, 100, 100, 100, 255);
            SDL_RenderFillRect(ref, &scrollbarRect);
        }
    }

    // Render vertical scrollbar
    if (showScrollbarY) {
        int maxScrollY = absContentSize.y - rect.h;
        if (maxScrollY > 0) {
            int scrollbarHeight = std::max(20, (rect.h * rect.h) / absContentSize.y);
            int scrollbarY = rect.y + (scrollY * (rect.h - scrollbarHeight)) / maxScrollY;

            SDL_Rect scrollbarRect = { rect.x + rect.w - 8, scrollbarY, 8, scrollbarHeight };
            SDL_SetRenderDrawColor(ref, 100, 100, 100, 255);
            SDL_RenderFillRect(ref, &scrollbarRect);
        }
    }
}

void GUILib::ScrollingFrame::updateScrollbars() {
    SDL_Rect rect = getRect();
    SDL_Point absContentSize = contentSize.getAbsoluteSize({ rect.w, rect.h });
    showScrollbarX = absContentSize.x > rect.w;
    showScrollbarY = absContentSize.y > rect.h;
}

void GUILib::ScrollingFrame::scroll(int deltaX, int deltaY) {
    SDL_Rect rect = getRect();
    SDL_Point absContentSize = contentSize.getAbsoluteSize({ rect.w, rect.h });

    if (showScrollbarX) {
        scrollX = clamp(scrollX + deltaX, 0, absContentSize.x - rect.w);
    }
    if (showScrollbarY) {
        scrollY = clamp(scrollY + deltaY, 0, absContentSize.y - rect.h);
    }
}

void GUILib::ScrollingFrame::handleEvent(const SDL_Event& event) {
    GuiObject::handleEvent(event);
    if (event.type == SDL_MOUSEWHEEL) {
        scroll(0, -event.wheel.y * 10);
    }
}

SDL_Color GUILib::ScrollingFrame::getScrollbarColor() const { return scrollingBarColor; }
void GUILib::ScrollingFrame::setScrollbarColor(SDL_Color color) { scrollingBarColor = color; }