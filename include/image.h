#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include "guiobject.h"


namespace GUILib {

	/// @brief Texture wrapper, but with more features.
	class Image : public GuiObject {
	protected:
		/// @brief The texture of the image.
		SDL_Texture* imageTexture;
		/// @brief The file path of the image.
		std::string filePath;
		/// @brief The previous file path of the image.
		std::string prevFilePath;
		
		Image(
			std::shared_ptr<GuiObject> parent,
			SDL_Renderer* renderer,
			UIUnit size = UIUnit(),
			UIUnit position = UIUnit(),
			std::string filePath = ""
		);
	public:
		Image();

		class Builder final : public GuiObject::Builder<Builder, Image> {};

		/// @brief Initializes the image.
		/// @param renderer The renderer to be used.
		void initialize(SDL_Renderer* renderer) override;

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

		/// @brief Returns the class name of the object.
		/// @return The class name.
		inline std::string getClassName() const override { return "Image"; };

		~Image() override;
	};
}


#endif /* IMAGE_H */
