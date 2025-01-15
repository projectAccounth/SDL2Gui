#include "image.h"

void Image::initialize(SDL_Renderer* renderer) {
	imageSurface = IMG_Load(filePath);
	if (imageSurface == nullptr) {
		std::cout << "A problem occurred when trying to create image. Error: " << SDL_GetError() << '\n';
		return;
	}
	imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);	
}

void Image::render(SDL_Renderer* renderer) {
	if (!isVisible()) return;

	if (filePath != prevFilePath) {
		SDL_DestroyTexture(imageTexture);
		imageSurface = IMG_Load(filePath);
		imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
		SDL_FreeSurface(imageSurface);
		prevFilePath = filePath;
	}

	SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
}

void Image::updatePath(std::string str) {
	filePath = str.c_str();
}

void Image::resize(int w, int h) {
	imageRect.w = w;
	imageRect.h = h;
}

void Image::move(int x, int y) {
	imageRect.x = x;
	imageRect.y = y;
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

void ImageManager::renderAll(SDL_Renderer* renderer) {
	for (auto& img : images) {
		img.render(renderer);
	}
}