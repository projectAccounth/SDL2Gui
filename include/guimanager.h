#pragma once

#include "guiobject.h"

class GuiManager {
private:
    std::unordered_map<std::string, std::unique_ptr<GuiObject>> objects;
    int idCounter = 0;

    std::unordered_map<std::string, std::function<std::unique_ptr<GuiObject>(SDL_Renderer*&)>> factory;

public:
    void registerClass(const std::string& className,
        std::function<std::unique_ptr<GuiObject>(SDL_Renderer*&)> creator);

    std::unique_ptr<GuiObject> addObject(const std::string& className, SDL_Renderer*& param);

    void renderAll(SDL_Renderer* renderer) const;

    GuiObject* getObject(const std::string& id);

    // Remove a GUI object by ID
    void removeObject(const std::string& id);

    void initialize();
};