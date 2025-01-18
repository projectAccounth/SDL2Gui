#include "guiobject.h"

void GuiObject::updateSize(SDL_Renderer* renderer) {
	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(renderer, &ws, &hs);

	if (!parent) {
		objRect.w = static_cast<int>(
			position.isUsingScale ? ws * size.sizeX : size.sizeX);
		objRect.h = static_cast<int>(
			position.isUsingScale ? hs * size.sizeY : size.sizeY);

		objRect.x = static_cast<int>(position.isUsingScale ? ws * position.sizeX : position.sizeX);
		objRect.y = static_cast<int>(position.isUsingScale ? hs * position.sizeY : position.sizeY);
	}
	else {
		auto prnt = parent.value();
		auto prntPos = prnt->position;
		auto prntSize = prnt->size;

		auto prntSizeX = prntSize.sizeX;
		auto prntSizeY = prntSize.sizeY;

		if (prnt->size.isUsingScale) {
			prntSizeX *= ws;
			prntSizeY *= hs;
		}

		if (prntPos.isUsingScale) {
			objRect.x = static_cast<int>(prntPos.sizeX * ws +
				(position.isUsingScale ? position.sizeX * prntSizeX : position.sizeX));
			objRect.y = static_cast<int>(prntPos.sizeY * hs +
				(position.isUsingScale ? position.sizeY * prntSizeY : position.sizeY));
		}

		objRect.w = static_cast<int>(size.sizeX * (size.isUsingScale ? prntSizeX : 1));
		objRect.h = static_cast<int>(size.sizeY * (size.isUsingScale ? prntSizeY : 1));
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
	parent(std::nullopt)
{}
GuiObject::GuiObject(
	UIUnit size, UIUnit position,
	std::optional<GuiObject*> parent,
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
	std::optional<GuiObject*> parent,
	SDL_Renderer* renderer, SDL_Color frameColor,
	bool isVisible, bool isActive
):
	GuiObject(size, position, parent, renderer, isVisible, isActive),
	frameColor(frameColor)
{}

void Frame::render(SDL_Renderer* renderer) {
	if (!(visible && active)) return;

	SDL_SetRenderDrawColor(renderer, frameColor.r, frameColor.g, frameColor.b, frameColor.a);
	SDL_RenderFillRect(renderer, &objRect);
}