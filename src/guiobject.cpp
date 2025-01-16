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
	objRect{ 0, 0, 0, 0 },
	visible(false),
	active(false)
{}
GuiObject::GuiObject(
	int x, int y, int w, int h,
	bool isVisible, bool isActive
):
	objRect{x, y, w, h},
	visible(isVisible),
	active(isActive)
{}