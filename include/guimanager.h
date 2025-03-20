#pragma once

#include "guiobject.h"


namespace GUILib {

    class GuiInstance {
    private:
        inline static std::unordered_map<
            std::string,
            std::function<std::unique_ptr<GuiObject>(GuiObject* parent, SDL_Renderer*&)>
        > factory;
    public:
        template <class T>
        void registerClass(
            const std::string& className
        ) {
            static_assert(std::is_base_of_v<GuiObject, T>, "T must inherit from GuiObject");
            factory[className] = [](GuiObject* parent, SDL_Renderer*& ref) -> std::unique_ptr<GuiObject> {
                return std::make_unique<T>(parent, ref);
                };
        }

        template <class T>
        std::unique_ptr<T> newObj(const std::string& className, SDL_Renderer*& ref) {
            static_assert(std::is_base_of_v<GuiObject, T>, "T must inherit from GuiObject");
            auto it = factory.find(className);
            if (it != factory.end()) {
                return std::unique_ptr<T>(static_cast<T*>(it->second(nullptr, ref).release()));
            }
            else {
                std::cerr << "Error: Class " << className << " not registered.\n";
                return nullptr;
            }
        }

        void initialize();
    };

}