#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include "guiobject.h"

// Images.
class Image : public GuiObject {
private:
	SDL_Surface* imageSurface;
	SDL_Texture* imageTexture;
public:
	const char* filePath;
	const char* prevFilePath;

	Image(const char* filePath, int width, int height, int locX, int locY);

	void initialize(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);

	void updatePath(std::string filePath);

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
