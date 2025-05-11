#include "guiobject.h"
#include "frame.h"

SDL_Renderer* nullrnd = nullptr;

static std::ostream& operator<<(std::ostream& os, const GUILib::UIUnit& u)
{
	os << std::boolalpha << "(x: " << u.sizeX << ", y: " << u.sizeY << ", isUsingScale: " << u.isUsingScale << ")";
	return os;
}

/// Recursive calculation
static std::pair<GUILib::UIUnit, GUILib::UIUnit> calculateChild(
	const GUILib::GuiObject& obj,
	const int& windowWidth,
	const int& windowHeight
) {
	// Base case: If no parent exists, calculate relative to the window
	const auto [objSize, objPos] = std::pair { obj.getSize(), obj.getPosition() };
	GUILib::UIUnit outPos{}, outSize{};

	if (!obj.hasParent()) {

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
	const auto& parent = obj.getParent(); // Will never be a dangling pointer
	auto [parentPos, parentSize] = calculateChild(*parent.lock(), windowWidth, windowHeight);

	// Calculate this object's position and size relative to the parent's
	outPos = {
		parentPos.sizeX +
		(objPos.isUsingScale ? parentSize.sizeX * objPos.sizeX : objPos.sizeX),
		parentPos.sizeY +
		(objPos.isUsingScale ? parentSize.sizeY * objPos.sizeY : objPos.sizeY),
		objPos.isUsingScale
	};
	outSize = {
		objSize.isUsingScale ? parentSize.sizeX * objSize.sizeX : objSize.sizeX,
		objSize.isUsingScale ? parentSize.sizeY * objSize.sizeY : objSize.sizeY,
		objSize.isUsingScale
	};

	if (const auto scrollingParent = parent.lock()) {
		if (const auto sp = std::dynamic_pointer_cast<GUILib::ScrollingFrame>(scrollingParent)) {
			// auto parentRect = scrollingParent->getRect();
        	outPos.sizeX -= sp->getScrollX();
			outPos.sizeY -= sp->getScrollY();
		}
    }

	return { outPos, outSize };
}

void GUILib::GuiObject::update(SDL_Renderer* renderer)
{
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

void GUILib::GuiObject::move(const UIUnit& newPos)
{
	position = newPos;
	int ws = 0, hs = 0;
	SDL_GetRendererOutputSize(ref, &ws, &hs);
	if (parent.lock()) {
		SDL_Rect parentSize = parent.lock()->getRect();
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

void GUILib::GuiObject::resize(const UIUnit& newSize)
{
	size = newSize;
	update(ref);
	trigger("onSizeChange", size);
}

SDL_Rect GUILib::GuiObject::getRect() const
{
	return objRect;
}

GUILib::UIUnit GUILib::GuiObject::getSize() const
{
	return size;
}

GUILib::UIUnit GUILib::GuiObject::getPosition() const
{
	return position;
}

void GUILib::GuiObject::handleEvent(const SDL_Event& event)
{
	update(ref);
	if (!active || !visible) {
		return;
	}
	for (const auto& child : children) {
		if (!child) continue;
		child->handleEvent(event);
	}

	if (!isDraggable()) return;

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
		if (!isDragging) break;

		const int offsetX = event.motion.x - dragOffsetX;
		const int offsetY = event.motion.y - dragOffsetY;

		const UIUnit newPos {
			position.isUsingScale ? static_cast<double>(offsetX) / static_cast<double>(ws) : offsetX,
			position.isUsingScale ? static_cast<double>(offsetY) / static_cast<double>(hs) : offsetY,
			position.isUsingScale
		};

		move(newPos);
		break;
	}
}

GUILib::GuiObject::GuiObject():
	size(UIUnit()),
	position(UIUnit()),
	objRect{ 0, 0, 0, 0 },
	visible(false),
	active(false),
	ref(nullrnd),
	canBeDragged(false),
	isDragging(false),
	dragOffsetX(0),
	dragOffsetY(0),
	shouldRenderChildren(true),
	renderingPivotOffset(),
	degreeRotation()
{}
GUILib::GuiObject::GuiObject(
	std::shared_ptr<GuiObject> parent,
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
	dragOffsetY(0),
	shouldRenderChildren(true),
	renderingPivotOffset(),
	degreeRotation()
{
	if (renderer)
		update(renderer);
	if (parent) {
		parent->addChild(shared_from_this());
		ref = parent->getCurrentRenderer();
	}
}

bool GUILib::GuiObject::isVisible() const
{
	return visible;
}
bool GUILib::GuiObject::isActive() const
{
	return active;
}

GUILib::GuiObject& GUILib::GuiObject::operator=(const GUILib::GuiObject& other)
{
	if (this == &other) return *this;
	ref = other.ref;

	size = other.size;
	position = other.position;

	parent = other.parent;
	children = other.children;

	canBeDragged = other.canBeDragged;
	dragOffsetX = other.dragOffsetX;
	dragOffsetY = other.dragOffsetY;
	isDragging = false;

	visible = other.visible;
	active = other.active;

	objRect = other.objRect;

	shouldRenderChildren = other.shouldRenderChildren;
	renderingPivotOffset = other.renderingPivotOffset;
	degreeRotation = other.degreeRotation;

	update(ref);

	return *this;
}


void GUILib::GuiObject::updateRenderer(SDL_Renderer*& renderer)
{
	if (renderer == ref) return;
	ref = renderer;
	if (parent.lock() && parent.lock()->getCurrentRenderer() != renderer) {
		parent.lock()->removeChild(shared_from_this());
	}

	for (const auto& child : children) {
		if (!child) continue;
		child->updateRenderer(renderer);
	}

	update(renderer);
    trigger("onRendererUpdate");
}

GUILib::GuiObject::~GuiObject()
{
	if (auto p = parent.lock())
		p->removeChild(shared_from_this());
}

void GUILib::GuiObject::setVisible(bool value)
{
    visible = value;
    trigger("onVisibilityChange");
}

void GUILib::GuiObject::toggleVisibility(bool value)
{
	visible = value;
	trigger("onVisibilityChange");
}

void GUILib::GuiObject::setActive(bool value)
{ 
	active = value;
	trigger("onActiveChange");
}

const std::weak_ptr<GUILib::GuiObject>& GUILib::GuiObject::getParent() const
{
	return parent;
}

bool GUILib::GuiObject::hasParent() const noexcept
{
	return parent.lock().get();
}

bool GUILib::GuiObject::setParent(const std::shared_ptr<GuiObject>& newParent)
{
    auto self = shared_from_this();
	bool success = false;

    if (newParent == parent.lock()) {
        // Already has this parent
        return success;
    }

    if (newParent && newParent->isDescendantOf(self)) {
        throw std::logic_error("Cannot set a descendant as parent (what the fuck are you doing?)");
    }

    // Detach from current parent
    if (auto oldParent = parent.lock()) {
        auto& siblings = oldParent->children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), self), siblings.end());
    }

    parent = newParent;
	success = true;

    // Attach to new parent
    if (newParent) {
        newParent->children.push_back(self);
    }

    trigger("onParentChange", success);
	return success;
}

bool GUILib::GuiObject::addChild(const std::shared_ptr<GuiObject>& child)
{
	if (!child) return false;

	bool success = false;

	if (child.get() == this) {
		throw std::logic_error("Cannot add self as child");
	}

	// Only reparent if not already a child of this
	if (child->parent.lock().get() != this) {
		child->setParent(shared_from_this());
		success = true;
	}
	trigger("onChildAdded", success);
	return success;
}

bool GUILib::GuiObject::removeChild(const std::shared_ptr<GuiObject>& child)
{
	if (!child) return false;

	bool success = false;

	if (child->parent.lock().get() == this) {
		success = child->setParent(nullptr);
	}
	trigger("onChildRemoved", success);
	return success;
}


bool GUILib::GuiObject::isAscendantOf(const std::shared_ptr<const GuiObject>& other) const
{
	auto current = other;
	while (current) {
		if (current.get() == this) {
			return true;
		}
		current = current->parent.lock();
	}
	return false;
}

bool GUILib::GuiObject::isDescendantOf(const std::shared_ptr<const GuiObject>& other) const
{
	return other->isAscendantOf(shared_from_this());
}

bool GUILib::GuiObject::isDraggable() const noexcept
{
	return canBeDragged;
}

void GUILib::GuiObject::setDraggable(bool value)
{
	canBeDragged = value;
}

void GUILib::GuiObject::render()
{
	if (!shouldRender()) return;
	if (!shouldRenderChildren) return;

    for (const auto& child : children) {
		if (!child) continue;
		child->updateRenderer(ref);
		child->render();
	}
}

SDL_Renderer* GUILib::GuiObject::getCurrentRenderer() const
{
	return ref;
}

bool GUILib::GuiObject::operator==(const GuiObject& other) const
{
	return this == &other;
}

bool GUILib::GuiObject::getChildrenRenderingState() const
{
	return shouldRenderChildren;
}

void GUILib::GuiObject::setChildrenRenderingState(bool value)
{
	shouldRenderChildren = value;
}

SDL_Point GUILib::UIUnit::getAbsoluteSize(const SDL_Point& containerSize) const
{
	if (isUsingScale) {
		return {static_cast<int>(sizeX * containerSize.x), static_cast<int>(sizeY * containerSize.y)};
	}
	return {static_cast<int>(sizeX), static_cast<int>(sizeY)};
}

double GUILib::Reserved::clamp(double val, double min, double max)
{
    return std::min(std::max(val, min), max);
}

bool GUILib::Reserved::isPointInRect(const SDL_Point& point, const SDL_Rect& rect) {
	return point.x >= rect.x && point.x <= rect.x + rect.w &&
		   point.y >= rect.y && point.y <= rect.y + rect.h;
}

GUILib::Reserved::TextureType GUILib::Reserved::createSolidBoxTexture(
	SDL_Renderer* r,
	SDL_Color c,
	int w, int h
) {
	if (!r) return { nullptr, SDL_DestroyTexture };

	SDL_Texture* t = SDL_CreateTexture(r,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		w, h
	);

	if (!t) return { nullptr, SDL_DestroyTexture };

	SDL_Texture* oldTarget = SDL_GetRenderTarget(r);

	SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(r, t);

	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	SDL_RenderClear(r);

	SDL_SetRenderTarget(r, oldTarget);

	return { t, SDL_DestroyTexture };
}

GUILib::UIUnit GUILib::GuiObject::getPivotOffset() const
{
	return renderingPivotOffset;
}

SDL_Point GUILib::GuiObject::getPivotOffsetPoint() const
{
	return renderingPivotOffset.getAbsoluteSize({ getRect().w, getRect().h });
}

void GUILib::GuiObject::setRotation(const double& rotation)
{
	degreeRotation = rotation;
	trigger("onRotationChange", rotation);
}

double GUILib::GuiObject::getRotation() const
{
	return degreeRotation;
}

void GUILib::GuiObject::setPivotOffset(const UIUnit& offset)
{
	renderingPivotOffset = offset;
	trigger("onPivotOffsetChange", offset);
}

std::pair<float, float> GUILib::Reserved::rotatePoint(float px, float py, float ox, float oy, float angle_rad)
{
    const float sinX = std::sin(angle_rad);
    const float cosX = std::cos(angle_rad);

    // Translate point to origin
    px -= ox;
    py -= oy;

    // Rotate
    float newX = px * cosX - py * sinX;
    float newY = px * sinX + py * cosX;

    // Translate back
    newX += ox;
    newY += oy;

    return {newX, newY};
}

bool GUILib::GuiObject::shouldRender() const
{
	const bool isVisibleAndRendererAvailable = isVisible() && ref != nullptr;
	const bool isParentVisible = parent.lock() && parent.lock()->isVisible();

	return hasParent() ? 
		isVisibleAndRendererAvailable && isParentVisible :
		isVisibleAndRendererAvailable;
}

bool GUILib::UIUnit::operator==(const UIUnit& other) const
{
	return sizeX == other.sizeX &&
		   sizeY == other.sizeY &&
		   isUsingScale == other.isUsingScale;
}

GUILib::GuiObject& GUILib::GuiObject::operator=(GuiObject&& other) noexcept
{
	ref = other.ref;
	other.ref = nullptr;

	size = other.size;
	position = other.position;

	other.size = other.position = UIUnit();

	parent = std::move(other.parent);
	children = std::move(other.children);

	canBeDragged = other.canBeDragged;

	dragOffsetX = other.dragOffsetX;
	dragOffsetY = other.dragOffsetY;

	other.dragOffsetX = other.dragOffsetY = 0;

	isDragging = false;

	visible = other.visible;
	active = other.active;

	other.visible = other.active = false;

	objRect = other.objRect;
	other.objRect = SDL_Rect();

	shouldRenderChildren = other.shouldRenderChildren;
	renderingPivotOffset = other.renderingPivotOffset;
	degreeRotation = other.degreeRotation;

	update(ref);

	return *this;
}

GUILib::GuiObject::GuiObject(GuiObject&& other) noexcept = default;

GUILib::GuiObject::GuiObject(const GuiObject& other) noexcept = default;

std::string GUILib::GuiObject::getEssentialInformation() const {
	std::ostringstream oss;
	oss << "GuiObject Debug Info:\n";
	oss << "  Name: " << name << "\n";
	oss << "  Class: " << getClassName() << "\n";
	oss << "  Position: " << position << "\n";
	oss << "  Size: " << size << "\n";
	oss << "  Drag Offset: (" << dragOffsetX << ", " << dragOffsetY << ")\n";
	oss << "  Rotation (deg): " << degreeRotation << "\n";
	oss << "  Pivot Offset: " << renderingPivotOffset << "\n";
	oss << "  Active: " << (active ? "true" : "false") << "\n";
	oss << "  Visible: " << (visible ? "true" : "false") << "\n";
	oss << "  Is Dragging: " << (isDragging ? "true" : "false") << "\n";
	oss << "  Can Be Dragged: " << (canBeDragged ? "true" : "false") << "\n";
	oss << "  Render Children: " << (shouldRenderChildren ? "true" : "false") << "\n";
	oss << "  Children count: " << children.size() << "\n";
	if (parent.lock()) {
		oss << "  Has Parent: true\n";
	}
	else {
		oss << "  Has Parent: false\n";
	}

	return oss.str();
}

void GUILib::GuiObject::initialize(SDL_Renderer* r)
{
	if (!r) return;
	if (r != ref) updateRenderer(r);
	if (const auto p = parent.lock()) {
		p->initialize(r);
	}
}
void GUILib::GuiObject::resetListeners(const std::string& eventName)
{
	events.reset(eventName);
}

