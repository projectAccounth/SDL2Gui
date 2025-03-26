#pragma once

#include "guiobject.h"

namespace GUILib {

    namespace {
        // helper function, __cplusplus refuses to work
        inline double clamp(double val, double min, double max) {
            return std::min(std::max(val, min), max);
        }
    }

    /// @brief A basic frame.
    /// @brief Can be used as a base for all frames.
    class Frame : public GuiObject {
    private:

        /// @brief The color of the frame.
        SDL_Color frameColor;

    public:
        Frame();
        Frame(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            SDL_Color frameColor = SDL_Color(),
            bool isVisible = true,
            bool isActive = true);

        /// @brief Changes the frame color.
        /// @param color The color to be applied.
        void setFrameColor(const SDL_Color& color);

        /// @brief Gets the current frame color.
        /// @return The current color.
        SDL_Color getFrameColor() const;

        /// @brief Renders the frame.
        void render() override;
    };

    /// @brief A scrollable frame.
    /// Shouldn't be used for now, as the implementation is still buggy.
    class ScrollingFrame : public Frame {
    private:

        /// @brief Full size of the scrollable content.
        UIUnit contentSize;               

        /// @brief Current scroll position.
        int scrollX,
            scrollY;

        /// @brief Whether the scrollbars are visible. Automatically adjusted.
        bool showScrollbarX,
             showScrollbarY;

        /// @brief The color of the scrollbar.
        SDL_Color scrollingBarColor;

        /// @brief Updates the scrollbars.
        void updateScrollbars();

        /// @brief Scrolls the content.
        /// @param deltaX The change in X.
        /// @param deltaY The change in Y.
        void scroll(int deltaX, int deltaY);

        /// @brief Renders the scrollbars.
        /// @param absContentSize The absolute content size of this (reference to contentSize).
        /// @param rect The rect of the object.
        void renderScrollbars(const SDL_Point& absContentSize, const SDL_Rect& rect);

       public:
        ScrollingFrame(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            UIUnit contentSize = UIUnit(),
            SDL_Color frameColor = { 255, 255, 255, 255 },
            bool isVisible = true,
            bool isActive = true
        );

        /// @brief Gets the color of the scrollbar.
        SDL_Color getScrollbarColor() const;

        /// @brief Sets the color of the scrollbar.
        /// @param color The new color of the scrollbar.
        void setScrollbarColor(SDL_Color color);

        /// @brief Sets the size of the content.
        /// @param newSize The new size of the content.
        void setContentSize(const UIUnit& newSize);

        /// @brief Gets the size of the content.
        /// @return The size of the content.
        inline UIUnit getContentSize() const { return contentSize; }

        /// @brief Renders the frame, together with the scrolling bar.
        void render() override;

        /// @brief Handles the event for the object.
        /// @param event The event to be passed.
        void handleEvent(const SDL_Event& event) override;
    };
}