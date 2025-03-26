#pragma once

#include "guiobject.h"


namespace GUILib {

    /// @brief A class to facilitate the creation of GUI objects.
    /// @brief Might be useful for creating GUI objects in a more organized manner, and dynamically.
    class GuiInstance {
    public:
        /// @brief Creates a GUI object of type T, must be derived from GuiObject.
        /// @param className The name of the class to be created.
        template <class T>
        std::unique_ptr<T> create(
            const std::string& className,
            SDL_Renderer*& ref
        ) {
            static_assert(std::is_base_of_v<GuiObject, T>, "T must inherit from GuiObject");
            return std::make_unique<T>(T(nullptr, ref));
        }
    };
}