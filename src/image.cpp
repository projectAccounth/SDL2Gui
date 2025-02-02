#include "image.h"

Image::Image(
	const char* filePath,
	std::optional<GuiObject*> parent,
	SDL_Renderer*& renderer,
	UIUnit position, UIUnit size
) :
	GuiObject(size, position, parent, renderer),
	imageSurface(nullptr),
	imageTexture(nullptr),
	filePath(filePath),
	prevFilePath(filePath)
{
	initialize(renderer);
}
	
void Image::initialize(SDL_Renderer* renderer) {
	imageSurface = IMG_Load(filePath.c_str());
	if (imageSurface == nullptr) {
		std::cout << "A problem occurred when trying to create image. Error: " << SDL_GetError() << '\n';
		return;
	}
	imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);	
}

void Image::render() {
	if (!isVisible() || (parent && !parent.value()->visible) || !imageTexture) return;

	SDL_RenderCopy(ref, imageTexture, NULL, &objRect);
}

void Image::updatePath(const std::string& str, SDL_Renderer* renderer) {
	prevFilePath = filePath;
	filePath = str;

	initialize(renderer);
}

std::string Image::getFilePath() const {
	return filePath;
}

std::string Image::previousFilePath() const {
	return prevFilePath;
}

Image::~Image() {
	SDL_DestroyTexture(imageTexture);
	SDL_FreeSurface(imageSurface);
}

bool Image::isVisible() const { return visible; }

void ImageManager::add(const Image& img) {
	images.push_back(img);
}

void ImageManager::initializeAll(SDL_Renderer* renderer) {
	for (auto& img : images) {
		img.initialize(renderer);
	}
}

void ImageManager::renderAll() {
	for (auto& img : images) {
		img.render();
	}
}