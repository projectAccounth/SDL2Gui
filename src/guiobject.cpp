#include "guiobject.h"

void GuiObject::updateSize(SDL_Renderer* renderer) {
	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(renderer, &ws, &hs);

	if (std::holds_alternative<SDL_Renderer*>(parent)) {
		objRect.w =
			position.isUsingScale ? ws * size.sizeX : size.sizeX;
		objRect.h =
			position.isUsingScale ? hs * size.sizeY : size.sizeY;

		objRect.x = position.isUsingScale ? ws * position.sizeX : position.sizeX;
		objRect.y = position.isUsingScale ? hs * position.sizeY : position.sizeY;
	}
	else {
		auto prnt = std::get<GuiObject&>(parent);
		auto prntPos = prnt.position;
		auto prntSize = prnt.size;

		auto prntSizeX = prnt.size.sizeX;
		auto prntSizeY = prnt.size.sizeY;

		if (prnt.size.isUsingScale) {
			prntSizeX *= ws;
			prntSizeY *= hs;
		}

		if (prntPos.isUsingScale) {
			objRect.x = prntPos.sizeX * ws +
				(position.isUsingScale ? position.sizeX * prntSizeX : position.sizeX);
			objRect.y = prntPos.sizeY * hs +
				(position.isUsingScale ? position.sizeY * prntSizeY : position.sizeY);
		}

		objRect.w = size.sizeX * (size.isUsingScale ? prntSizeX : 1);
		objRect.h = size.sizeY * (size.isUsingScale ? prntSizeY : 1);
	}
}

void GuiObject::move(const UIUnit& newPos, SDL_Renderer* renderer) {
	position = newPos;
	updateSize(renderer);
}

void GuiObject::resize(const UIUnit& newSize, SDL_Renderer* renderer) {
	size = newSize;
	updateSize(renderer);
}

SDL_Rect GuiObject::getSize() const {
	return objRect;
}

GuiObject::GuiObject():
	size(UIUnit()),
	position(UIUnit()),
	objRect{ 0, 0, 0, 0 },
	visible(false),
	active(false),
	parent(nullptr)
{}
GuiObject::GuiObject(
	UIUnit size, UIUnit position,
	std::variant<SDL_Renderer*, GuiObject&> parent,
	SDL_Renderer* renderer,
	bool isVisible, bool isActive
) :
	position(position),
	size(size),
	visible(isVisible),
	active(isActive),
	parent(parent)
{
	updateSize(renderer);
}

Frame::Frame(): GuiObject(), frameColor(SDL_Color()) {}
Frame::Frame(
	UIUnit size, UIUnit position,
	std::variant<SDL_Renderer*, GuiObject&> parent,
	SDL_Renderer* renderer, SDL_Color frameColor = SDL_Color(),
	bool isVisible = true, bool isActive = true
):
	GuiObject(size, position, parent, renderer, isVisible, isActive),
	frameColor(frameColor)
{}