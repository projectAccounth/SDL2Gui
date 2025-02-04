#pragma once

#include "guiobject.h"

class GuiInstance {
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<GuiObject>(SDL_Renderer*&)>> factory;
public:
    void registerClass(const std::string& className,
        std::function<std::unique_ptr<GuiObject>(SDL_Renderer*&)> creator);

    std::unique_ptr<GuiObject> newObj(const std::string& className, SDL_Renderer*& param);

    void initialize();
};