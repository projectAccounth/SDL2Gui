#include "guiobject.h"

SDL_Renderer* nullrnd = nullptr;

std::pair<GUILib::UIUnit, GUILib::UIUnit> calculateChild(
	const GUILib::GuiObject& obj,
	const int& windowWidth,
	const int& windowHeight
) {
	// Base case: If no parent exists, calculate relative to the window
	const auto [objSize, objPos] = std::pair<GUILib::UIUnit, GUILib::UIUnit>{ obj.getSize(), obj.getPosition() };
	if (!obj.hasParent()) {
		GUILib::UIUnit outPos, outSize;

		outSize.sizeX = static_cast<int>(
			objSize.isUsingScale ? windowWidth * objSize.sizeX : objSize.sizeX);
		outSize.sizeY = static_cast<int>(
			objSize.isUsingScale ? windowHeight * objSize.sizeY : objSize.sizeY);

		outPos.sizeX = static_cast<int>(
			objPos.isUsingScale ? windowWidth * objPos.sizeX : objPos.sizeX);
		outPos.sizeY = static_cast<int>(
			objPos.isUsingScale ? windowHeight * objPos.sizeY : objPos.sizeY);

		outPos.isUsingScale = objPos.isUsingScale;
		outSize.isUsingScale = objSize.isUsingScale;

		return { outPos, outSize };
	}

	// Recursive case: Calculate parent's position and size first
	const auto parent = obj.getParent(); // Will never be a dangling pointer
	auto [parentPos, parentSize] = calculateChild(*parent, windowWidth, windowHeight);

	// Calculate this object's position and size relative to the parent's
	GUILib::UIUnit outPos, outSize;

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

	outPos.isUsingScale = objPos.isUsingScale;
	outSize.isUsingScale = objSize.isUsingScale;

	return { outPos, outSize };
}

void GUILib::GuiObject::update(SDL_Renderer* renderer) {
	if (!renderer)
		return;

	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(renderer, &ws, &hs);

	auto [calculatedPos, calculatedSize] = calculateChild(*this, ws, hs);

	objRect.x = static_cast<int>(calculatedPos.sizeX);
	objRect.y = static_cast<int>(calculatedPos.sizeY);
	objRect.w = static_cast<int>(calculatedSize.sizeX);
	objRect.h = static_cast<int>(calculatedSize.sizeY);
}

void GUILib::GuiObject::move(const UIUnit& newPos) {
	position = newPos;
	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(ref, &ws, &hs);
	if (parent && parent != nullptr) {
		SDL_Rect parentSize = parent->getRect();
		objRect.x = static_cast<int>(position.isUsingScale ? parentSize.w * position.sizeX : position.sizeX);
		objRect.y = static_cast<int>(position.isUsingScale ? parentSize.h * position.sizeY : position.sizeY);
	}
	else {
		objRect.x = static_cast<int>(position.isUsingScale ? position.sizeX * ws : position.sizeX);
		objRect.y = static_cast<int>(position.isUsingScale ? position.sizeY * hs : position.sizeY);
	}

	update(ref);
	trigger("onPositionChange", position);
}

void GUILib::GuiObject::resize(const UIUnit& newSize) {
	size = newSize;
	update(ref);
	trigger("onSizeChange", size);
}

SDL_Rect GUILib::GuiObject::getRect() const {
	return objRect;
}

GUILib::UIUnit GUILib::GuiObject::getSize() const {
	return size;
}

GUILib::UIUnit GUILib::GuiObject::getPosition() const {
	return position;
}

void GUILib::GuiObject::handleEvent(const SDL_Event& event) {
	update(ref);
	if (!canBeDragged || !visible) {
		return;
	}
	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(ref, &ws, &hs);
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
			trigger("onDragging", event.button.x, event.button.y);
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
			int offsetX = event.motion.x - dragOffsetX;
			int offsetY = event.motion.y - dragOffsetY;
			newPos.sizeX = position.isUsingScale ? static_cast<double>(offsetX) / static_cast<double>(ws) : offsetX;
			newPos.sizeY = position.isUsingScale ? static_cast<double>(offsetY) / static_cast<double>(hs) : offsetY;
			newPos.isUsingScale = position.isUsingScale;
			move(newPos);
		}
		break;

	default:
		break;
	}
}

GUILib::GuiObject::GuiObject():
	size(UIUnit()),
	position(UIUnit()),
	objRect{ 0, 0, 0, 0 },
	visible(false),
	active(false),
	parent(nullptr),
	ref(nullrnd),
	canBeDragged(false),
	isDragging(false),
	dragOffsetX(0),
	dragOffsetY(0)
{}
GUILib::GuiObject::GuiObject(
	GuiObject* parent,
	SDL_Renderer*& renderer,
	UIUnit size,
	UIUnit position,
	bool isVisible, bool isActive
) :
	position(position),
	size(size),
	objRect({ 0, 0, 0, 0 }),
	visible(isVisible),
	active(isActive),
	parent(parent),
	ref(renderer),
	canBeDragged(false),
	isDragging(false),
	dragOffsetX(0),
	dragOffsetY(0)
{
	if (renderer)
		update(renderer);
}

bool GUILib::GuiObject::isVisible() const {
	return visible;
}
bool GUILib::GuiObject::isActive() const {
	return active;
}

GUILib::Frame::Frame(): GuiObject(), frameColor(SDL_Color()) {}
GUILib::Frame::Frame(
	GuiObject* parent,
	SDL_Renderer*& renderer,
	UIUnit size,
	UIUnit position,
	SDL_Color frameColor,
	bool isVisible, bool isActive
):
	GuiObject(parent, renderer, size, position, isVisible, isActive),
	frameColor(frameColor)
{}

void GUILib::Frame::render() {
	if (!(visible && active)) return;

	SDL_SetRenderDrawColor(ref, frameColor.r, frameColor.g, frameColor.b, frameColor.a);
	SDL_RenderFillRect(ref, &objRect);
}

GUILib::GuiObject& GUILib::GuiObject::operator=(const GUILib::GuiObject& other) {
	if (this == &other) return *this;
	ref = other.ref;

	size = other.size;
	position = other.position;

	parent = other.parent;

	canBeDragged = other.canBeDragged;
	dragOffsetX = other.dragOffsetX;
	dragOffsetY = other.dragOffsetY;
	isDragging = false;

	visible = other.visible;
	active = other.active;

	objRect = other.objRect;

	update(ref);

	return *this;
}