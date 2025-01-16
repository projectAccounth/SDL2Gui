#include "buttonManager.h"

void ButtonManager::add(const ButtonType& btn) {
	buttons.push_back(btn);
}

void ButtonManager::renderAll(SDL_Renderer *renderer) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) { btn.render(renderer); }, button);
	}
}

void ButtonManager::handleAllEvent(SDL_Event& e) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) { btn.handleEvents(e); }, button);
	}
}

void ButtonManager::initializeAll(SDL_Renderer *renderer) {
	for (auto& button : buttons) {
		const bool isTextButton =
			std::is_same_v<std::decay_t<decltype(button)>, TextButton>;
		const bool isImageButton =
			std::is_same_v<std::decay_t<decltype(button)>, ImageButton>;
		std::visit([&](auto& btn) {
			if constexpr (isTextButton) btn.loadText(renderer);
			if constexpr (isImageButton) btn.initialize(renderer);
		}, button);
	}
}