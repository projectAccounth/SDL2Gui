#include "guiobject.h"

SDL_Renderer* nullrnd = nullptr;

std::pair<UIUnit, UIUnit> calculateChild(
	const GuiObject& obj,
	const int& windowWidth,
	const int& windowHeight
) {
	// Base case: If no parent exists, calculate relative to the window
	const auto [objSize, objPos] = std::pair<UIUnit, UIUnit>{ obj.getSize(), obj.getPosition() };
	if (!obj.parent) {
		UIUnit outPos, outSize;

		outSize.sizeX = static_cast<int>(
			objSize.isUsingScale ? windowWidth * objSize.sizeX : objSize.sizeX);
		outSize.sizeY = static_cast<int>(
			objSize.isUsingScale ? windowHeight * objSize.sizeY : objSize.sizeY);

		outPos.sizeX = static_cast<int>(
			objPos.isUsingScale ? windowWidth * objPos.sizeX : objPos.sizeX);
		outPos.sizeY = static_cast<int>(
			objPos.isUsingScale ? windowHeight * objPos.sizeY : objPos.sizeY);

		return { outPos, outSize };
	}

	// Recursive case: Calculate parent's position and size first
	const GuiObject* parent = obj.parent.value();
	auto [parentPos, parentSize] = calculateChild(*parent, windowWidth, windowHeight);

	// Calculate this object's position and size relative to the parent's
	UIUnit outPos, outSize;

	outSize.sizeX = static_cast<int>(
		objSize.isUsingScale ? parentSize.sizeX * objSize.sizeX : objSize.sizeX);
	outSize.sizeY = static_cast<int>(
		objSize.isUsingScale ? parentSize.sizeY * objSize.sizeY : objSize.sizeY);

	outPos.sizeX = static_cast<int>(
		parentPos.sizeX +
		(objPos.isUsingScale ? parentSize.sizeX * objPos.sizeX : objPos.sizeX));
	outPos.sizeY = static_cast<int>(
		parentPos.sizeY +
		(objPos.isUsingScale ? parentSize.sizeY * objPos.sizeY : objPos.sizeY));

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

void GuiObject::move(const UIUnit& newPos) {
	position = newPos;
	if (parent) {
		SDL_Rect parentSize = parent.value()->getRect();
		objRect.x = static_cast<int>(position.isUsingScale ? parentSize.w * position.sizeX : position.sizeX);
		objRect.y = static_cast<int>(position.isUsingScale ? parentSize.h * position.sizeY : position.sizeY);
	}
	else {
		objRect.x = static_cast<int>(position.isUsingScale ? 0 : position.sizeX);
		objRect.y = static_cast<int>(position.isUsingScale ? 0 : position.sizeY);
	}

	update(ref);
}

void GuiObject::resize(const UIUnit& newSize) {
	size = newSize;
	update(ref);
}

SDL_Rect GuiObject::getRect() const {
	return objRect;
}

UIUnit GuiObject::getSize() const {
	return size;
}

UIUnit GuiObject::getPosition() const {
	return position;
}

void GuiObject::handleEvent(const SDL_Event& event) {
	if (!canBeDragged) return;
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT &&
			event.button.x >= objRect.x &&
			event.button.x <= objRect.x + objRect.w &&
			event.button.y >= objRect.y &&
			event.button.y <= objRect.y + objRect.h
			) {
			isDragging = true;
			dragOffsetX = event.button.x - objRect.x;
			dragOffsetY = event.button.y - objRect.y;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT) {
			isDragging = false;
		}
		break;

	case SDL_MOUSEMOTION:
		if (isDragging) {
			UIUnit newPos;
			newPos.sizeX = event.motion.x - dragOffsetX;
			newPos.sizeY = event.motion.y - dragOffsetY;
			move(newPos); // Update the position
		}
		break;

	default:
		break;
	}
}

GuiObject::GuiObject():
	size(UIUnit()),
	position(UIUnit()),
	objRect{ 0, 0, 0, 0 },
	visible(false),
	active(false),
	parent(std::nullopt),
	ref(nullrnd),
	canBeDragged(false)
{}
GuiObject::GuiObject(
	UIUnit size, UIUnit position,
	std::optional<GuiObject*> parent,
	SDL_Renderer*& renderer,
	bool isVisible, bool isActive
) :
	position(position),
	size(size),
	visible(isVisible),
	active(isActive),
	parent(parent),
	ref(renderer),
	canBeDragged(false)
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
	SDL_Renderer*& renderer, SDL_Color frameColor,
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