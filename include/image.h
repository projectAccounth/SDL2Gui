#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"

// Images.
class Image {
private:
	SDL_Surface* imageSurface;
	SDL_Texture* imageTexture;
	SDL_Rect imageRect;
public:
	
	bool visible;
	const char* filePath;
	const char* prevFilePath;

	Image(const char* filePath, int width, int height, int locX, int locY):
		imageSurface(nullptr),
		imageTexture(nullptr),
		imageRect{locX, locY, width, height},
		visible(true),
		filePath(filePath),
		prevFilePath(filePath)
	{}

	void initialize(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);

	void updatePath(std::string filePath);

	void resize(int w, int h);

	void move(int x, int y);

	bool isVisible() const;
};

class ImageManager {
public:
	std::vector<Image> images;

	void add(const Image& img);

	void initializeAll(SDL_Renderer* renderer);

	void renderAll(SDL_Renderer* renderer);
};


#endif /* IMAGE_H */
