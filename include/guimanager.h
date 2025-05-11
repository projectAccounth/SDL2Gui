#pragma once

#include "guiobject.h"

namespace GUILib {

    /// @brief A class to facilitate the creation of GUI objects.
    /// @brief Useful for creating GUI objects in a more organized manner, and dynamically.
    /// Might be deprecated in the near future in favor of object builders.
    class GuiInstance {
    public:
        /// @brief Creates a GUI object of type T, must be derived from GuiObject.
        /// @param ref The renderer to use for the object.
        template <class T>
        std::shared_ptr<T> create(
            SDL_Renderer*& ref
        ) {
            static_assert(std::is_base_of_v<GuiObject, T>, "T must inherit from GuiObject");
            return T::Builder().setRenderer(ref).build();
        }
    };

    template<typename _Pt, typename _Bt>
    void check_shared_ptr_subclass() {
        static_assert(
            std::is_base_of_v<_Bt, typename _Pt::element_type>,
            "The shared_ptr does not point to a subclass of the expected base type.");
    }


    /// @brief A class to facilitate the scene rendering.
    /// @brief Might be useful for bulk rendering, and I don't recommend using this now.
    class SceneManager {
    private:
        /// @brief The renderer to use for the scene.
        SDL_Renderer* ref;
        /// @brief The list of objects to render. (Only the references).
        std::vector<std::shared_ptr<GuiObject>> objects;

    public:
        /// @brief Creates a scene manager.
        /// @param ref The renderer to use for the scene.
        explicit SceneManager(SDL_Renderer* ref) : ref(ref) {}

        /// @brief Adds an object to the scene.
        /// @param obj The object to add.
        void add(std::shared_ptr<GuiObject> obj);

        template <typename... GuiObjects>
        void addBulk(GuiObjects... args) {
            (check_shared_ptr_subclass<GuiObjects, GuiObject>(), ...);

            (objects.push_back(args), ...);
        }

        /// @brief Renders all objects in the scene.
        void render();

        /// @brief Handles all event for objects.
        /// @param e The event to be handled.
        void handleEvent(const SDL_Event& e) const;

        /// @brief Clears the scene.
        /// @param obj The object to clear.
        void clear(std::shared_ptr<GuiObject> obj);

        /// @brief Clears all objects in the scene.
        void clearAll();

        /// @brief Updates the renderer.
        /// @param r The renderer.
        void updateRenderer(SDL_Renderer* r);

    	~SceneManager();
    };
}