#pragma once

#include "guiobject.h"

namespace GUILib {

    /// @brief A class to facilitate the creation of GUI objects.
    /// @brief Might be useful for creating GUI objects in a more organized manner, and dynamically.
    class GuiInstance {
    public:
        /// @brief Creates a GUI object of type T, must be derived from GuiObject.
        /// @param ref The renderer to use for the object.
        template <class T>
        std::unique_ptr<T> create(
            SDL_Renderer*& ref
        ) {
            static_assert(std::is_base_of_v<GuiObject, T>, "T must inherit from GuiObject");
            return std::make_unique<T>(T(nullptr, ref));
        }
    };

    /// @brief A class to facilitate the scene rendering.
    /// @brief Might be useful for bulk rendering, and I don't recommend using this now.
    class SceneManager {
    private:
        /// @brief The renderer to use for the scene.
        SDL_Renderer*& ref;
        /// @brief The list of objects to render. (Only the references).
        std::vector<GuiObject*> objects;
    public:
        /// @brief Creates a scene manager.
        /// @param ref The renderer to use for the scene.
        SceneManager(SDL_Renderer*& ref) : ref(ref) {}

        /// @brief Adds an object to the scene.
        /// @param obj The object to add.
        void add(GuiObject* obj);

        template <typename... GuiObjects>
        void addBulk(GuiObjects... args) {
            static_assert((std::is_base_of_v<
                               std::remove_pointer_t<GuiObject*>,
                               std::remove_pointer_t<GuiObjects>> &&
                           ...),
                          "All arguments must be pointers to subclasses of GuiObject");

            (objects.push_back(args), ...);
        }

        /// @brief Renders all objects in the scene.
        void render();

        /// @brief Handles all event for objects.
        /// @param e The event to be handled.
        void handleEvent(const SDL_Event& e);

        /// @brief Clears the scene.
        /// @param obj The object to clear.
        void clear(GuiObject* obj);

        /// @brief Clears all objects in the scene.
        void clearAll();

        ~SceneManager();
    };
}