#pragma once

#include "types.h"

typedef struct UIUnit {
	int sizeX, sizeY;
	bool isUsingScale;
} UIUnit;

class GuiObject {
protected:
	SDL_Rect objRect;
public:

	UIUnit position;
	UIUnit size;
	
	std::variant<SDL_Window*, GuiObject&> parent;

	bool visible, active;

	GuiObject();
	GuiObject(
		UIUnit size, UIUnit position,
		bool isVisible, bool isActive
	);

	SDL_Rect getSize() const;

	void move(int x, int y);

	void resize(int x, int y);
};
