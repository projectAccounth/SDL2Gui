#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "types.h"
#include "guiobject.h"
#include "textBox.h"

namespace GUILib {

    /// @brief Abstract, practically useless button class.
    /// @brief Serves as a base for buttons.
    class Button : public GuiObject {
    protected:
        /// @brief The action when clicked.
        /// @deprecated Migrate to Events API.
        std::function<void()> buttonAction,
                              hoverAction;

        /// @brief Checks if the button is clicked.
        bool isClicked(int x, int y);
        /// @brief Checks if the button is hovered.
        void checkHover(int mouseX, int mouseY);

        /// @brief Whether the button is hovered.
        bool hovered;
        /// @brief The class name.
		static inline const std::string CLASS_NAME = "Button";
    public:
        Button();
        Button(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit()
        );

        /// @deprecated Migrate to the event system.
        /// @brief Setting the function responsible for clicking.
        inline void setAction(const std::function<void()>& actionFunction) {
            buttonAction = actionFunction;
        }
        /// @deprecated Migrate to the event system.
        /// @brief Setting the function responsible for hovering.
        inline void setHoverAction(const std::function<void()>& actionFunction) {
            buttonAction = actionFunction;
        }

        /// @brief Handles all event for the button.
        /// @param e The event to be handled.
        void handleEvent(const SDL_Event& e) override;

        /// @brief Checks if the button is hovered.
        /// @return The value.
        inline bool isHovered() const { return hovered; }

        /// @brief Returns the class name of the object.
		/// @return The class name.
        inline std::string getClassName() const override { return "Button"; };

        virtual ~Button() = default;
    };

    /// @brief A basic text button for anything.
    class TextButton : public Button {
    protected:
        static int nextId;
        /// @brief The texture of the text.
        SDL_Texture* textTexture;
        /// @brief The font of the text.
        TTF_Font* textFont;
        /// @brief The alignment of the text.
        HorizontalTextAlign xAlign;
        /// @brief The alignment of the text.
        VerticalTextAlign yAlign;

        /// @brief The text of the button.
        std::string text;
        /// @brief The color of the button.
        SDL_Color buttonColor;
        /// @brief The color of the text.
        SDL_Color textColor;
        /// @brief The color of the hovered button.
        SDL_Color hoverColor;

        /// @brief The "ID" of the button, can be used for querying.
        int id;

        /// @brief The class name.
		static inline const std::string CLASS_NAME = "TextButton";
    public:
        TextButton();
        TextButton(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            SDL_Color buttonColor = SDL_Color(),
            SDL_Color hoverColor = SDL_Color(),
            SDL_Color textColor = { 0, 0, 0, 255 },
            std::string text = "",
            TTF_Font* textFont = nullptr,
            HorizontalTextAlign alignX = HorizontalTextAlign::CENTER,
            VerticalTextAlign alignY = VerticalTextAlign::CENTER
        );

        /// @brief Preloading text (must be called before rendering)
        void initialize(SDL_Renderer*& renderer);

        /// @brief Renders the button.
        void render() override;

        /// @brief Gets the ID of the button.
        /// @return The ID.
        int getId() const;

        /// @brief Changes text color.
        /// @param color The new color.
        void changeTextColor(
            const SDL_Color& color
        );

        /// @brief Changes hover color.
        /// @param color The new color.
        void changeHoverColor(
            const SDL_Color& color
        );

        /// @brief Changes button color.
        /// @param color The new color.
        void changeButtonColor(
            const SDL_Color& color
        );

        /// @brief Gets the text color.
        /// @return The text color.
        SDL_Color getTextColor() const;

        /// @brief Gets the hover color.
        /// @return The hover color.
        SDL_Color getHoverColor() const;

        /// @brief Gets the hover color.
        /// @return The hover color.
        SDL_Color getButtonColor() const;

        /// @brief Modifies the text.
        /// @param str The text that will be assigned.
        void setText(
            const std::string& str
        );

        /// @brief Returns the current text.
        /// @return The text;
        std::string getText() const;

        /// @brief Changes the reference to the font
        /// @param font The reference.
        void changeFont(
            TTF_Font* font
        );

        /// @brief Returns the class name of the object.
		/// @return The class name.
		inline std::string getClassName() const override { return "TextButton"; };

        ~TextButton();
    };
    
    /// @brief A basic image button wrapper.
    class ImageButton : public Button {
    protected:
        /// @brief The texture of the button.
        SDL_Texture* buttonTexture;
        /// @brief The hover texture of the button.
        SDL_Texture* hoverTexture;
        
        static int nextId;

        /// @brief The ID of the button.
        int id;
        /// @brief The default image path.
        std::string defaultImgPath;
        /// @brief The hover image path.
        std::string hoverImgPath;

        /// @brief The class name.
		static inline const std::string CLASS_NAME = "ImageButton";
    public:

        ImageButton();
        ImageButton(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            std::string defaultImageFilePath = "",
            std::string hoverImageFilePath = ""
        );

        /// @brief Initializes the button's components.
        void initialize(
            SDL_Renderer*& renderer
        );

        /// @brief Renders the button.
        void render() override;

        /// @brief Returns the ID.
        /// @return The ID.
        int getId() const;

        /// @brief Updates the hover image path.
        /// @param updatedPath The new path.
        void updateHoverImgPath(
            const char* updatedPath
        );

        /// @brief Updates the default image path.
        /// @param updatedPath The new path.
        void updateDefaultImgPath(
            const char* updatedPath
        );

        /// @brief Returns the class name of the object.
		/// @return The class name.
		inline std::string getClassName() const override { return "ImageButton"; };

        ~ImageButton();
    };

    /// @brief A basic check box.
    /// @brief Inherits TextButton.
    class CheckBox : public TextButton {
    private:
        /// @brief The symbol of the box.
        char boxSymbol;
        /// @brief The checked state of the box.
        bool checked;

        /// @brief The class name.
		static inline const std::string CLASS_NAME = "CheckBox";
    public:
        CheckBox();
        CheckBox(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            TTF_Font* textFont = nullptr,
            SDL_Color boxColor = SDL_Color(),
            SDL_Color textColor = SDL_Color(),
            char symbol = 'X'
        );

        /// @brief Toggles the checked state.
        void toggleChecked();

        /// @brief Sets the checked state.
        /// @param val The new state.
        void toggleChecked(bool val);

        /// @brief Checks if the box is checked.
        /// @return The value.
        bool isChecked() const;

        void setAction(
            const std::function<void()>& buttonAction
        ) = delete;

        void setHoverAction(
            const std::function<void()>& buttonAction
        ) = delete;

        /// @brief Changes the symbol of the box.
        void changeSymbol(const char& symbol);

        /// @brief Gets the symbol of the box.
        /// @brief The symbol.
        char getSymbol() const;

        /// @brief Handles the event.
        /// @param event The event to be handled.
        void handleEvent(const SDL_Event& event) override;

        /// @brief Returns the class name of the object.
		/// @return The class name.
		inline std::string getClassName() const override { return "CheckBox"; };
    };

}


#endif /* BUTTON_H */

