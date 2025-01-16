#pragma once

#include "types.h"

class GuiObject {
protected:
	SDL_Rect objRect;
public:

	bool visible, active;

	GuiObject();
	GuiObject(
		int x, int y, int w, int h,
		bool isVisible, bool isActive
	);

	SDL_Rect getSize() const;

	void move(int x, int y);

	void resize(int x, int y);
};
