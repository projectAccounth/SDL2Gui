#pragma once

#include "guiobject.h"
#include "frame.h"
#include "types.h"

namespace GUILib {
    enum class DragDirection {
        HORIZONTAL,
        VERTICAL
    };

    class Slider : public Frame {
    protected:
        /// @brief The offset of the drag.
        int offsetX, offsetY;
        /// @brief The last mouse position.
        int lastMouseX, lastMouseY;
        /// @brief Whether the user is dragging the scrollbar.
        bool draggingX, draggingY;

        /// @brief Renders the handle.
        void renderHandle();

        /// @brief The color of the handle.
        SDL_Color handleColor;
        /// @brief The rect of the handle.
        SDL_Rect handleRect;

        /// @brief The size of the handle.
        int handleSize;

        /// @brief The direction of the handle.
        DragDirection direction;

        /// @brief The class name.
		static inline const std::string CLASS_NAME = "Slider";
    public:
        Slider(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            SDL_Color frameColor = SDL_Color(),
            SDL_Color handleColor = SDL_Color{ 200, 200, 200, 255 }
        );

        /// @brief Sets the direction of the slider.
        /// @param dir The direction of the slider.
        void setDirection(DragDirection dir);

        /// @brief Gets the direction of the slider.
        /// @return The direction of the slider.
        DragDirection getDirection() const;

        /// @brief Renders the slider.
        void render() override;

        /// @brief Sets the color of the handle.
        /// @param color The new color of the handle.
        void setHandleColor(SDL_Color color);

        /// @brief Gets the color of the handle.
        /// @return The color of the handle.
        SDL_Color getHandleColor() const;

        /// @brief Sets the size of the handle.
        /// @param size The new size of the handle.
        void setHandleSize(int size);
        
        /// @brief Gets the size of the handle.
        /// @return The size of the handle.
        int getHandleSize() const;

        /// @brief Handles all events.
        /// @param event The event.
        void handleEvent(const SDL_Event& event) override;

        /// @brief Returns the class name of the object.
		/// @return The class name.
		inline std::string getClassName() const override { return "Slider"; };
    protected:
        /// @brief Unused function.
        /// @deprecated Not used. Remove this.
        void updateHandlePosition();

        /// @brief Moves the handle by a delta.
        /// @param delta The delta to move the handle by.
        void moveHandle(int delta);
    };
}