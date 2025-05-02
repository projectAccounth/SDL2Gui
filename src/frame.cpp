#include "frame.h"

GUILib::Frame::Frame() :
    frameColor() {}
GUILib::Frame::Frame(
	std::shared_ptr<GuiObject> parent,
    SDL_Renderer* renderer,
    const UIUnit& size,
    const UIUnit& position,
    const SDL_Color& frameColor,
    const bool& isVisible, const bool& isActive
) :
    GuiObject(parent, renderer, size, position, isVisible, isActive),
    frameColor(frameColor) {}

void GUILib::Frame::render()
{
    if (!shouldRender()) return;

	const auto frameTexture = Reserved::createSolidBoxTexture(ref, frameColor, objRect.w, objRect.h);

    if (frameTexture) {
		SDL_RenderCopyEx(ref, frameTexture.get(), nullptr, &objRect, degreeRotation, nullptr, SDL_FLIP_NONE);
    }

    GuiObject::render();
}

void GUILib::Frame::setFrameColor(const SDL_Color& color) { frameColor = color; }
SDL_Color GUILib::Frame::getFrameColor() const { return frameColor; }

GUILib::Frame& GUILib::Frame::operator=(Frame&& other) noexcept
{
    if (&other == this) return *this;

    GuiObject::operator=(other);
    this->frameColor = other.frameColor;

    return *this;
}

GUILib::Frame& GUILib::Frame::operator=(const Frame& other) noexcept
{
    if (&other == this) return *this;

    GuiObject::operator=(other);
    this->frameColor = other.frameColor;

    return *this;
}

GUILib::Frame::Frame(Frame&& other) noexcept:
	frameColor(other.frameColor)
{
	
}

GUILib::Frame::Frame(const Frame& other) noexcept = default;