#include "frame.h"

using namespace GUILib::Reserved;

GUILib::ScrollingFrame::ScrollingFrame(
    std::shared_ptr<GuiObject> parent,
    SDL_Renderer* renderer,
    UIUnit size,
    UIUnit position,
    UIUnit contentSize,
    SDL_Color frameColor,
    bool isVisible,
    bool isActive) : Frame(parent, renderer, size, position, frameColor, isVisible, isActive),
                     contentSize(contentSize),
                     scrollX(0),
                     scrollY(0),
                     showScrollbarX(false),
                     showScrollbarY(false),
                     scrollingSpeed(10),
                     scrollingBarColor(),
                     handleRectX(),
                     handleRectY() 
{
}

GUILib::ScrollingFrame::ScrollingFrame() = default;


void GUILib::ScrollingFrame::setContentSize(const UIUnit& newSize)
{
    if (newSize.isUsingScale == size.isUsingScale) {
        contentSize.sizeX = std::max(newSize.sizeX, size.sizeX);
        contentSize.sizeY = std::max(newSize.sizeY, size.sizeY);
    } else if (newSize.isUsingScale) {
        contentSize.sizeX = std::max(newSize.sizeX * size.sizeX, size.sizeX);
        contentSize.sizeY = std::max(newSize.sizeY * size.sizeY, size.sizeY);
    } else {
        contentSize.sizeX = std::max(newSize.sizeX, size.sizeX * newSize.sizeX);
        contentSize.sizeY = std::max(newSize.sizeY, size.sizeY * newSize.sizeY);
    }
    // Holy shit
    contentSize.isUsingScale = size.isUsingScale;
    updateScrollbars();
}

void GUILib::ScrollingFrame::render()
{
    if (!shouldRender()) return;

    Frame::render();  // Render base frame

    shouldRenderChildren = false;

    const SDL_Rect clipRect = getRect();

    const SDL_Point absContentSize = contentSize.getAbsoluteSize({clipRect.w, clipRect.h});

    SDL_RenderSetClipRect(ref, &objRect);

    for (const auto& child : children) {
        child->render();
    }

    SDL_RenderSetClipRect(ref, nullptr);

    updateScrollbars();

    renderScrollbars(absContentSize, clipRect);
}

void GUILib::ScrollingFrame::renderScrollbars(const SDL_Point& absContentSize, const SDL_Rect& rect)
{
    SDL_SetRenderDrawColor(ref,
        scrollingBarColor.r,
        scrollingBarColor.g,
        scrollingBarColor.b,
        scrollingBarColor.a
    );  // Dark gray scrollbars (by default)
    if (showScrollbarX) {
        if (const int maxScrollX = absContentSize.x - rect.w; maxScrollX > 0) {
            const int scrollbarWidth = std::max(20, (rect.w * rect.w) / absContentSize.x);
            const int scrollbarX = rect.x + (scrollX * (rect.w - scrollbarWidth)) / maxScrollX;

            handleRectX = {scrollbarX, rect.y + rect.h - 8, scrollbarWidth, 8};
            SDL_SetRenderDrawColor(
                ref,
                scrollingBarColor.r,
                scrollingBarColor.g,
                scrollingBarColor.b,
                scrollingBarColor.a
            );
            SDL_RenderFillRect(ref, &handleRectX);
        }
    }

    // Render vertical scrollbar
    if (showScrollbarY) {
        int maxScrollY = absContentSize.y - rect.h;
        if (maxScrollY > 0) {
            int scrollbarHeight = std::max(20, (rect.h * rect.h) / absContentSize.y);
            int scrollbarY = rect.y + (scrollY * (rect.h - scrollbarHeight)) / maxScrollY;

            handleRectY = {rect.x + rect.w - 8, scrollbarY, 8, scrollbarHeight};
            SDL_SetRenderDrawColor(
                ref,
                scrollingBarColor.r,
                scrollingBarColor.g,
                scrollingBarColor.b,
                scrollingBarColor.a
            );
            SDL_RenderFillRect(ref, &handleRectY);
        }
    }
}

void GUILib::ScrollingFrame::updateScrollbars() {
    const SDL_Rect rect = getRect();
    const auto [absContentSizeX, absContentSizeY] = contentSize.getAbsoluteSize({rect.w, rect.h});
    showScrollbarX = absContentSizeX > rect.w;
    showScrollbarY = absContentSizeY > rect.h;
}

void GUILib::ScrollingFrame::scroll(int deltaX, int deltaY) {
    const SDL_Rect rect = getRect();
    const auto [absContentSizeX, absContentSizeY] = contentSize.getAbsoluteSize({ rect.w, rect.h });

    if (showScrollbarX) {
        scrollX = static_cast<int>(clamp(scrollX + deltaX, 0, absContentSizeX - rect.w));
    }
    if (showScrollbarY) {
        scrollY = static_cast<int>(clamp(scrollY + deltaY, 0, absContentSizeY - rect.h));
    }
}

void GUILib::ScrollingFrame::handleEvent(const SDL_Event& event)
{
    GuiObject::handleEvent(event);

    if (!isActive()) return;

    const SDL_Rect rect = getRect();
    SDL_Point absContentSize = contentSize.getAbsoluteSize({rect.w, rect.h});

    if (event.type == SDL_MOUSEWHEEL) {
        scroll(0, -event.wheel.y * scrollingSpeed);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        const int mouseX = event.button.x;
        const int mouseY = event.button.y;

        if (showScrollbarX &&
            isPointInRect({mouseX, mouseY}, handleRectX)) 
        {
            draggingX = true;
            lastMouseX = mouseX;
        }
        if (showScrollbarY &&
            isPointInRect({mouseX, mouseY}, handleRectY)) 
        {
            draggingY = true;
            lastMouseY = mouseY;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        draggingX = draggingY = false;
    } else if (event.type == SDL_MOUSEMOTION) {
        if (draggingX) {
            const int deltaX = event.motion.x - lastMouseX;
            scroll(deltaX, 0);
            lastMouseX = event.motion.x;
        }
        if (draggingY) {
            const int deltaY = event.motion.y - lastMouseY;
            scroll(0, deltaY);
            lastMouseY = event.motion.y;
        }
    }
}

SDL_Color GUILib::ScrollingFrame::getScrollbarColor() const { return scrollingBarColor; }
void GUILib::ScrollingFrame::setScrollbarColor(SDL_Color color) { scrollingBarColor = color; }

void GUILib::ScrollingFrame::setScrollingSpeed(int speed) {
    scrollingSpeed = speed;
}

int GUILib::ScrollingFrame::getScrollX() const {
    return scrollX;
}

int GUILib::ScrollingFrame::getScrollY() const {
    return scrollY;
}

int GUILib::ScrollingFrame::getScrollingSpeed() const {
	return scrollingSpeed;
}

GUILib::ScrollingFrame& GUILib::ScrollingFrame::operator=(ScrollingFrame&& other) noexcept
{
    if (&other == this) return *this;

    Frame::operator=(other);

    handleRectX = other.handleRectX;
    handleRectY = other.handleRectY;
    scrollingBarColor = other.scrollingBarColor;
    showScrollbarX = other.showScrollbarX;
    showScrollbarY = other.showScrollbarY;
    contentSize = other.contentSize;
    scrollX = other.scrollX;
    scrollY = other.scrollY;
    lastMouseX = other.lastMouseX;
    lastMouseY = other.lastMouseY;
    draggingX = other.draggingX;

    return *this;
}

GUILib::ScrollingFrame& GUILib::ScrollingFrame::operator=(const ScrollingFrame& other)
{
    if (&other == this) return *this;

    Frame::operator=(other);

    handleRectX = other.handleRectX;
    handleRectY = other.handleRectY;
    scrollingBarColor = other.scrollingBarColor;
    showScrollbarX = other.showScrollbarX;
    showScrollbarY = other.showScrollbarY;
    contentSize = other.contentSize;
    scrollX = other.scrollX;
    scrollY = other.scrollY;
    lastMouseX = other.lastMouseX;
    lastMouseY = other.lastMouseY;
    draggingX = other.draggingX;

    return *this;
}

GUILib::ScrollingFrame::ScrollingFrame(const ScrollingFrame&) = default;
GUILib::ScrollingFrame::ScrollingFrame(ScrollingFrame&&) noexcept = default;