#include "image.h"

GUILib::Image::Image(
	GuiObject* parent,
	SDL_Renderer*& renderer,
	UIUnit size,
	UIUnit position,
	std::string filePath
) :
	GuiObject(parent, renderer, size, position),
	imageTexture(nullptr),
	filePath(filePath),
	prevFilePath(filePath)
{
	initialize(renderer);
	className = "Image";
}
	
void GUILib::Image::initialize(SDL_Renderer*& renderer) {
	if (!renderer)
		return;

	updateRenderer(renderer);

	auto fPath = filePath.c_str();
	SDL_Surface* imageSurface = IMG_Load(fPath);
	if (imageSurface == nullptr) {
		std::cout << "A problem occurred when trying to create image. Error: " << SDL_GetError() << '\n';
		return;
	}
	imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);
}

void GUILib::Image::render() {
	if (!isVisible() || (parent && !parent->isVisible()) || !imageTexture || !ref) return;
	GuiObject::render();

	update(ref); // special handling

    SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Point rotPivot = getPivotOffsetPoint();

    SDL_RenderCopyEx(ref, imageTexture, NULL, &objRect, degreeRotation, &rotPivot, flip);
}

void GUILib::Image::updatePath(const std::string& str) {
	prevFilePath = filePath;
	filePath = str;

	initialize(ref);
	trigger("onPathChange", str);
}

std::string GUILib::Image::getFilePath() const {
	return filePath;
}

std::string GUILib::Image::previousFilePath() const {
	return prevFilePath;
}

GUILib::Image::~Image() {
	if (imageTexture)
		SDL_DestroyTexture(imageTexture);
}

void GUILib::ImageManager::add(const Image& img) {
	images.push_back(img);
}

void GUILib::ImageManager::initializeAll(SDL_Renderer*& renderer) {
	for (auto& img : images) {
		img.initialize(renderer);
	}
}

void GUILib::ImageManager::renderAll() {
	for (auto& img : images) {
		img.render();
	}
}

static SDL_Texture* copyTexture(SDL_Renderer* renderer, SDL_Texture* source) {
	if (!source) return nullptr;

	int width, height;
	Uint32 format;
	SDL_QueryTexture(source, &format, nullptr, &width, &height);

	SDL_Texture* newTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
	if (!newTexture) return nullptr;

	SDL_SetRenderTarget(renderer, newTexture);
	SDL_RenderCopy(renderer, source, nullptr, nullptr);
	SDL_SetRenderTarget(renderer, nullptr);

	return newTexture;
}

GUILib::Image& GUILib::Image::operator=(const Image& other) {
	if (this == &other) return *this;

	GuiObject::operator=(other);

	if (imageTexture) SDL_DestroyTexture(imageTexture);

	imageTexture = ref ? copyTexture(ref, other.imageTexture) : nullptr;

	filePath = other.filePath;
	prevFilePath = other.prevFilePath;

	return *this;
}

GUILib::Image::Image(const Image& other) :
	GuiObject(other),
	filePath(other.filePath),
	prevFilePath(other.prevFilePath),
	imageTexture(nullptr)
{}