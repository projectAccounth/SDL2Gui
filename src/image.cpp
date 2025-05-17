#include "Image.h"

#include <complex>

GUILib::Image::Image(
	std::shared_ptr<GuiObject> parent,
	SDL_Renderer* renderer,
	UIUnit size,
	UIUnit position,
	std::string filePath
) :
	GuiObject(parent, renderer, size, position),
	imageTexture(),
	filePath(filePath),
	prevFilePath(filePath)
{
	initialize(renderer);
}

GUILib::Image::Image(): imageTexture() {};
	
void GUILib::Image::initialize(SDL_Renderer* renderer) 
{
	GuiObject::initialize(renderer);
	if (!renderer)
		return;

	if (filePath.empty()) {
		std::cout << "The file path is empty. Set it to something before using.\n";
	}

	if (imageTexture) SDL_DestroyTexture(imageTexture);

	const auto fPath = filePath.c_str();
	SDL_Surface* imageSurface = IMG_Load(fPath);
	if (imageSurface == nullptr) {
		std::cout << "A problem occurred when trying to create image. Error: " << SDL_GetError() << '\n';
		return;
	}
	imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);
}

void GUILib::Image::render() {
	if (!shouldRender()) return;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Point rotPivot = getPivotOffsetPoint();

	if (!imageTexture) initialize(ref);

    SDL_RenderCopyEx(ref, imageTexture, nullptr, &objRect, degreeRotation, &rotPivot, flip);

	GuiObject::render();
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