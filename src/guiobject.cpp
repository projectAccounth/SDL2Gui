#include "guiobject.h"

SDL_Renderer* nullrnd = nullptr;

std::pair<UIUnit, UIUnit> calculateChild(
	const GuiObject& obj,
	const int& windowWidth,
	const int& windowHeight
) {
	// Base case: If no parent exists, calculate relative to the window
	if (!obj.parent) {
		UIUnit outPos, outSize;

		outSize.sizeX = static_cast<int>(
			obj.size.isUsingScale ? windowWidth * obj.size.sizeX : obj.size.sizeX);
		outSize.sizeY = static_cast<int>(
			obj.size.isUsingScale ? windowHeight * obj.size.sizeY : obj.size.sizeY);

		outPos.sizeX = static_cast<int>(
			obj.position.isUsingScale ? windowWidth * obj.position.sizeX : obj.position.sizeX);
		outPos.sizeY = static_cast<int>(
			obj.position.isUsingScale ? windowHeight * obj.position.sizeY : obj.position.sizeY);

		return { outPos, outSize };
	}

	// Recursive case: Calculate parent's position and size first
	const GuiObject* parent = obj.parent.value();
	auto [parentPos, parentSize] = calculateChild(*parent, windowWidth, windowHeight);

	// Calculate this object's position and size relative to the parent's
	UIUnit outPos, outSize;

	outSize.sizeX = static_cast<int>(
		obj.size.isUsingScale ? parentSize.sizeX * obj.size.sizeX : obj.size.sizeX);
	outSize.sizeY = static_cast<int>(
		obj.size.isUsingScale ? parentSize.sizeY * obj.size.sizeY : obj.size.sizeY);

	outPos.sizeX = static_cast<int>(
		parentPos.sizeX +
		(obj.position.isUsingScale ? parentSize.sizeX * obj.position.sizeX : obj.position.sizeX));
	outPos.sizeY = static_cast<int>(
		parentPos.sizeY +
		(obj.position.isUsingScale ? parentSize.sizeY * obj.position.sizeY : obj.position.sizeY));

	return { outPos, outSize };
}

void GuiObject::update(SDL_Renderer* renderer) {
	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(renderer, &ws, &hs);

	auto [calculatedPos, calculatedSize] = calculateChild(*this, ws, hs);

	objRect.x = static_cast<int>(calculatedPos.sizeX);
	objRect.y = static_cast<int>(calculatedPos.sizeY);
	objRect.w = static_cast<int>(calculatedSize.sizeX);
	objRect.h = static_cast<int>(calculatedSize.sizeY);
}

void GuiObject::move(const UIUnit& newPos, SDL_Renderer* renderer) {
	position = newPos;
	update(renderer);
}

void GuiObject::resize(const UIUnit& newSize, SDL_Renderer* renderer) {
	size = newSize;
	update(renderer);
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
	parent(std::nullopt),
	ref(nullrnd)
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
	parent(parent),
	ref(renderer)
{
	update(renderer);
}

bool GuiObject::isVisible() const {
	return visible;
}
bool GuiObject::isActive() const {
	return active;
}
void GuiObject::toggleVisiblility(bool value) {
	visible = value;
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