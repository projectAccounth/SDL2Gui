#pragma once

#include "types.h"

typedef struct UIUnit {
	double sizeX, sizeY;
	bool isUsingScale;
} UIUnit;

class GuiObject {
protected:
	SDL_Rect objRect;
	void update(SDL_Renderer* renderer);
	SDL_Renderer*& ref;
public:

	UIUnit position;
	UIUnit size;
	
	std::optional<GuiObject*> parent;

	bool visible, active;

	GuiObject();
	GuiObject(
		UIUnit size,
		UIUnit position,
		std::optional<GuiObject*> parent,
		SDL_Renderer* renderer,
		bool isVisible = true,
		bool isActive = true
	);

	SDL_Rect getSize() const;

	void move(const UIUnit& newPos, SDL_Renderer* renderer);

	void resize(const UIUnit& newSize, SDL_Renderer* renderer);

	bool isVisible() const;

	bool isActive() const;

	void toggleVisiblility(bool value);
};

class Frame : public GuiObject {
private:
	SDL_Color frameColor;
public:
	Frame();
	Frame(
		UIUnit size, UIUnit position,
		std::optional<GuiObject*> parent,
		SDL_Renderer* renderer, SDL_Color frameColor = SDL_Color(),
		bool isVisible = true, bool isActive = true
	);

	void render(SDL_Renderer* renderer);
};
