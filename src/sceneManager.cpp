#include "guimanager.h"

void GUILib::SceneManager::add(GuiObject* obj) {
    if (!obj || obj->hasParent()) return;
    objects.push_back(obj);
}

void GUILib::SceneManager::render() {
    if (!ref) return;
    for (auto& obj : objects) {
        if (!obj) continue;
        obj->updateRenderer(ref);
        obj->render();
    }
}

void GUILib::SceneManager::clear(GuiObject* obj) {
    auto it = std::remove(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it, objects.end());
    }
}

void GUILib::SceneManager::clearAll() {
    objects.clear();
}

void GUILib::SceneManager::handleEvent(const SDL_Event& e) {
    for (auto& obj : objects) {
        if (!obj) continue;
        obj->handleEvent(e);
    }
}

GUILib::SceneManager::~SceneManager() {
    for (auto& obj : objects) {
        if (!obj) continue;
        // How the hell do I handle stack objects?
        delete obj;
    }
    objects.clear();
}