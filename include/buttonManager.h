#pragma once

#ifndef BUTTONMGR_H
#define BUTTONMGR_H

#include "types.h"
#include "button.h"

using buttonType = std::variant<TextButton, ImageButton>;

class ButtonManager {
public:
	std::vector<buttonType> buttons;

	void add(buttonType btn);

	void renderAll(SDL_Renderer *renderer);

	void handleAllEvent(SDL_Event e);

	void initializeAll(SDL_Renderer *renderer);
};

#endif /* BUTTONMGR_H */