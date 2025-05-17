#pragma once

#include "guiobject.h"

namespace GUILib {

    /// @brief A basic frame.
    /// @brief Can be used as a base for all frames.
    class Frame : public GuiObject {
    protected:

        /// @brief The color of the frame.
        SDL_Color frameColor;

        /// @brief The class name.
        static inline const std::string CLASS_NAME = "Frame";
        
        Frame(
            std::shared_ptr<GuiObject> parent,
            SDL_Renderer* renderer,
            const UIUnit& size = UIUnit(),
            const UIUnit& position = UIUnit(),
            const SDL_Color& frameColor = SDL_Color(),
            const bool& isVisible = true,
            const bool& isActive = true);
    public:
		Frame();

        class Builder final : public GuiObject::Builder<Builder, Frame> {};
        
        Frame(const Frame& other) noexcept;
        Frame(Frame&& other) noexcept;

        Frame& operator=(Frame&&) noexcept;
        Frame& operator=(const Frame&) noexcept;

        /// @brief Changes the frame color.
        /// @param color The color to be applied.
        void setFrameColor(const SDL_Color& color);

        /// @brief Gets the current frame color.
        /// @return The current color.
        [[nodiscard]] SDL_Color getFrameColor() const;

        /// @brief Renders the frame.
        void render() override;

        /// @brief Returns the class name of the object.
        /// @return The class name.
        inline std::string getClassName() const override { return "Frame"; }

        ~Frame() override = default; // Nothing to clean up
    };

    /// @brief A scrollable frame with handle support.
    class ScrollingFrame final : public Frame {
    private:

        /// @brief Full size of the scrollable content.
        UIUnit contentSize = UIUnit();               

        /// @brief Current scroll position.
        int scrollX = 0,
            scrollY = 0;

        /// @brief Whether the scrollbars are visible. Automatically adjusted.
        bool showScrollbarX = true,
             showScrollbarY = true;

        /// @brief The color of the scrollbar.
        SDL_Color scrollingBarColor;

        /// @brief The scrolling speed.
        int scrollingSpeed = 10;

        /// @brief The last mouse position.
        int lastMouseX, lastMouseY;
        /// @brief Whether the user is dragging the scrollbar.
        bool draggingX, draggingY;
        
        SDL_Rect handleRectX, handleRectY;

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

        /// @brief The class name.
    	static inline const std::string CLASS_NAME = "ScrollingFrame";

        int scrollbarWidth = 20;

        ScrollingFrame(
            std::shared_ptr<GuiObject> parent,
            SDL_Renderer* renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            UIUnit contentSize = UIUnit(),
            SDL_Color frameColor = { 255, 255, 255, 255 },
            bool isVisible = true,
            bool isActive = true
        );
    public:
		ScrollingFrame();

        class Builder final : public GuiObject::Builder<Builder, ScrollingFrame> {};

        /// @brief Gets the color of the scrollbar.
        /// @returns The scrollbar color.
        SDL_Color getScrollbarColor() const;

        /// @brief Gets the width of the scrollbar, in pixels.
        /// @returns The width.
        [[nodiscard]] int getScrollbarWidth() const;

        /// @brief Sets the width of the scrollbar, in pixels.
        /// @param val The value.
        void setScrollbarWidth(const int& val);

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

        /// @brief Gets the scrolling speed of this frame (px/ms).
        int getScrollingSpeed() const;

        /// @brief Sets the scrolling speed of this frame (px/ms).
        /// @param speed The new scrolling speed.
        void setScrollingSpeed(int speed);

        /// @brief Gets the current X position of the handle.
        /// @return The X position of the handle relative to the progress.
        int getScrollX() const;
        
        /// @brief Gets the current Y position of the handle.
        /// @return The Y position of the handle relative to the progress.
        int getScrollY() const;

        /// @brief Returns the class name of the object.
        /// @return The class name.
        inline std::string getClassName() const override { return "ScrollingFrame"; }

        ScrollingFrame& operator=(const ScrollingFrame&);
        ScrollingFrame(const ScrollingFrame& other);
        ScrollingFrame(ScrollingFrame&&) noexcept;
        ScrollingFrame& operator=(ScrollingFrame&&) noexcept;
    };
}