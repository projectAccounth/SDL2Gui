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

	bool isDragging = false;
	int dragOffsetX = 0;
	int dragOffsetY = 0;
public:
	// Use move() for this.
	UIUnit position;
	// Use resize() for this.
	UIUnit size;

	std::optional<GuiObject*> parent;

	bool visible, active;

	bool canBeDragged;

	GuiObject();
	GuiObject(
		UIUnit size,
		UIUnit position,
		std::optional<GuiObject*> parent,
		SDL_Renderer*& renderer,
		bool isVisible = true,
		bool isActive = true
	);

	SDL_Rect getRect() const;

	UIUnit getSize() const;

	UIUnit getPosition() const;

	void move(const UIUnit& newPos);

	void resize(const UIUnit& newSize);

	bool isVisible() const;

	bool isActive() const;

	void toggleVisiblility(bool value);

	void handleEvent(const SDL_Event& event);

	virtual void render() = 0;

	virtual ~GuiObject() = default;
};

class Frame : public GuiObject {
private:
public:
	Frame();
	Frame(
		UIUnit size, UIUnit position,
		std::optional<GuiObject*> parent,
		SDL_Renderer*& renderer, SDL_Color frameColor = SDL_Color(),
		bool isVisible = true, bool isActive = true
	);
	SDL_Color frameColor;
	void render() override;
};
