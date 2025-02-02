#pragma once

#ifndef BUTTONMGR_H
#define BUTTONMGR_H

#include "types.h"
#include "button.h"

using ButtonType = std::variant<TextButton, ImageButton>;

class ButtonManager {
public:
	std::vector<ButtonType> buttons;

	void add(const ButtonType& btn);

	void renderAll();

	void handleAllEvent(SDL_Event& e);

	void initializeAll(SDL_Renderer *renderer);
};

#endif /* BUTTONMGR_H */