#pragma once

#ifndef BUTTONMGR_H
#define BUTTONMGR_H

#include "types.h"
#include "button.h"

namespace GUILib {

	/// @brief The tuple.
	using ButtonType = std::variant<TextButton, ImageButton>;

	/// @brief A fancy std::vector<Button> wrapper, but with more features.
	class ButtonManager {
	public:
		std::vector<ButtonType> buttons;

		void add(const ButtonType& btn);

		void renderAll();

		void handleAllEvent(SDL_Event& e);

		void initializeAll(SDL_Renderer*& renderer);
	};

}

#endif /* BUTTONMGR_H */