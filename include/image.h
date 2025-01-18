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
	std::string filePath;
	std::string prevFilePath;
public:

	Image(const char* filePath,
		std::optional<GuiObject*> parent,
		SDL_Renderer* renderer,
		UIUnit position, UIUnit size
	);

	void initialize(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);

	void updatePath(const std::string& filePath, SDL_Renderer* renderer);

	std::string previousFilePath() const;

	std::string getFilePath() const;

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
