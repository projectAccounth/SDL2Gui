#include "guiobject.h"

void GuiObject::move(int x, int y) {
	objRect.x = x;
	objRect.y = y;
}

void GuiObject::resize(int w, int h) {
	objRect.w = w;
	objRect.h = h;
}

SDL_Rect GuiObject::getSize() const {
	return objRect;
}

GuiObject::GuiObject():
	size(UIUnit()),
	position(UIUnit()),
	objRect{ 0, 0, 0, 0 },
	visible(false),
	active(false)
{}
GuiObject::GuiObject(
	UIUnit size, UIUnit position,
	bool isVisible, bool isActive
) :
	position(position),
	size(size),
	visible(isVisible),
	active(isActive)
{
	objRect = SDL_Rect();

	objRect.x = position.isUsingScale ? position.sizeX
}