#pragma once

#include "types.h"

typedef struct UIUnit {
	int sizeX, sizeY;
	bool isUsingScale;
} UIUnit;

class GuiObject {
protected:
	SDL_Rect objRect;
	void updateSize(SDL_Renderer* renderer);
public:

	UIUnit position;
	UIUnit size;
	
	std::variant<SDL_Renderer*, GuiObject&> parent;

	bool visible, active;

	GuiObject();
	GuiObject(
		UIUnit size, UIUnit position,
		std::variant<SDL_Renderer*, GuiObject&> parent,
		SDL_Renderer* renderer,
		bool isVisible = true, bool isActive = true
	);

	SDL_Rect getSize() const;

	void move(const UIUnit& newPos, SDL_Renderer* renderer);

	void resize(const UIUnit& newSize, SDL_Renderer* renderer);
};

class Frame : public GuiObject {
private:
	SDL_Color frameColor;
public:
	Frame();
	Frame(
		UIUnit size, UIUnit position,
		std::variant<SDL_Renderer*, GuiObject&> parent,
		SDL_Renderer* renderer, SDL_Color frameColor = SDL_Color(),
		bool isVisible = true, bool isActive = true
	);
};
