#include "buttonManager.h"

void GUILib::ButtonManager::add(const ButtonType& btn) {
	buttons.push_back(btn);
}

void GUILib::ButtonManager::renderAll() {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) { btn.render(); }, button);
	}
}

void GUILib::ButtonManager::handleAllEvent(SDL_Event& e) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) { btn.handleEvent(e); }, button);
	}
}

void GUILib::ButtonManager::initializeAll(SDL_Renderer*& renderer) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) {
			btn.initialize(renderer);
		}, button);
	}
}