#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include "guiobject.h"


namespace GUILib {

	/// @brief Texture wrapper, but with more features.
	class Image : public GuiObject {
	private:
		/// @brief The texture of the image.
		SDL_Texture* imageTexture;
		/// @brief The file path of the image.
		std::string filePath;
		/// @brief The previous file path of the image.
		std::string prevFilePath;
	public:

		Image(
			GuiObject* parent,
			SDL_Renderer*& renderer,
			UIUnit size = UIUnit(),
			UIUnit position = UIUnit(),
			std::string filePath = ""
		);

		/// @brief Initializes the image.
		/// @param renderer The renderer to be used.
		void initialize(SDL_Renderer*& renderer);

		/// @brief Renders the image.
		void render() override;

		/// @brief Updates the path of the image.
		void updatePath(
			const std::string& filePath
		);

		/// @brief Returns the previous file path.
		/// @return The previous file path.
		std::string previousFilePath() const;

		/// @brief Returns the file path.
		/// @return The file path.
		std::string getFilePath() const;

		Image(const Image&);
		Image& operator=(const Image&);

		~Image();
	};

	/// @brief A fancy `std::vector<Image>`. Subject to removal.
	class ImageManager {
	public:
		ImageManager() = default;
		~ImageManager() = default;

		/// @brief A vector of images.
		std::vector<Image> images;

		/// @brief Adds an image to the manager.
		/// @param img The image to be added.
		void add(const Image& img);

		/// @brief Initializes all images.
		void initializeAll(SDL_Renderer*& renderer);

		/// @brief Renders all images.
		void renderAll();
	};

}


#endif /* IMAGE_H */
