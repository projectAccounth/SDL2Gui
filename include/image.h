#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include "guiobject.h"


namespace GUILib {

	// Texture wrapper, but with more features.
	class Image : public GuiObject {
	private:
		SDL_Texture* imageTexture;
		std::string filePath;
		std::string prevFilePath;
	public:

		Image(
			GuiObject* parent,
			SDL_Renderer*& renderer,
			UIUnit size = UIUnit(),
			UIUnit position = UIUnit(),
			std::string filePath = ""
		);

		void initialize(SDL_Renderer*& renderer);

		void render() override;

		void updatePath(
			const std::string& filePath
		);

		std::string previousFilePath() const;

		std::string getFilePath() const;

		Image(const Image&);
		Image& operator=(const Image&);

		~Image();
	};

	class ImageManager {
	public:
		std::vector<Image> images;

		void add(const Image& img);

		void initializeAll(SDL_Renderer*& renderer);

		void renderAll();
	};

}


#endif /* IMAGE_H */
