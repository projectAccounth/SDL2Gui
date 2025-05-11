#include "guimanager.h"

void GUILib::SceneManager::add(std::shared_ptr<GuiObject> obj) {
    if (!obj || obj->hasParent()) return; // Push the hell parent in
    objects.push_back(obj);
}

void GUILib::SceneManager::render() {
    if (!ref) return;
    for (const auto& obj : objects) {
        if (!obj) continue;
        obj->updateRenderer(ref);
        obj->render();
    }
}

void GUILib::SceneManager::clear(std::shared_ptr<GuiObject> obj) {
    auto it = std::remove(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it, objects.end());
    }
}

void GUILib::SceneManager::clearAll()
{
    objects.clear();
}

void GUILib::SceneManager::handleEvent(const SDL_Event& e)  
{ 
   objects.erase(std::remove_if(objects.begin(), objects.end(),
       [](const std::shared_ptr<GuiObject>& p) { return p == nullptr; }),
       objects.end());
   for (auto& obj : objects) {  
       if (!obj) continue;  
       obj->handleEvent(e);  
   }  
}



GUILib::SceneManager::~SceneManager() {
    objects.clear();
}

void GUILib::SceneManager::updateRenderer(SDL_Renderer* r)
{
    ref = r;
}

void GUILib::SceneManager::initialize(SDL_Renderer* r) const
{
	for (const auto& object : objects)
	{
        if (!object) continue;
        object->initialize(r);
	}
}
