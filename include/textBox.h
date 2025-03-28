#pragma once

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "types.h"
#include "guiobject.h"


namespace GUILib {

    /// @brief A basic text box.
    /// @brief Can be used for texts, just remember to set the background colors' alpha channel to 0.
    class TextBox : public GuiObject {
    protected:
        /// @brief The reference to the font.
        TTF_Font* textFont;
        /// @brief The texture of the text.
        SDL_Texture* textTexture;
        /// @brief The alignment of the text.
        HorizontalTextAlign xAlign;
        /// @brief The alignment of the text.
        VerticalTextAlign yAlign;

        /// @brief The splitted lines for rendering.
        std::vector<std::string> lines;

        /// @brief The height of the lines based on the size of it.
        int lineHeight() const;

        /// @brief Splits the text into lines.
        std::vector<std::string> splitTextIntoLines(std::string& text, int maxWidth);

        /// @brief Renders the text box.
        std::string text;
        /// @brief The color of the box.
        SDL_Color boxColor;
        /// @brief The color of the text.
        SDL_Color textColor;

        /// @brief The class name.
		static inline const std::string CLASS_NAME = "TextBox";
    public:

        TextBox(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            SDL_Color boxColor = SDL_Color(),
            std::string text = "",
            SDL_Color textColor = { 0, 0, 0, 255 },
            TTF_Font* textFont = nullptr,
            HorizontalTextAlign alignX = HorizontalTextAlign::CENTER,
            VerticalTextAlign alignY = VerticalTextAlign::CENTER
        );

        /// @brief Renders the text box.
        void render() override;

        /// @brief Updates the text of the text box.
        /// @param textToUpdate The new text to be displayed.
        /// @deprecated Use setText instead.
        void updateText(const char* textToUpdate);

        /// @brief Adjusts the text alignment.
        /// @param alignX The horizontal alignment.
        /// @param alignY The vertical alignment.
        void adjustTextAlignment(
            const HorizontalTextAlign& alignX,
            const VerticalTextAlign& alignY
        );

        /// @brief Returns the background color.
        /// @return The background color.
        SDL_Color getBoxColor() const;

        /// @brief Returns the text color.
        /// @return The text color.
        SDL_Color getTextColor() const;

        /// @brief Returns the text of the text box.
        /// @return The text of the text box.
        std::string getText() const;

        /// @brief Changes the background color.
        void setBoxColor(const SDL_Color& color);

        /// @brief Changes the text color.
        void setTextColor(const SDL_Color& color);

        /// @brief Changes the text of the text box.
        void setText(const std::string& str);

        /// @brief Changes the font of the text.
        void changeFont(TTF_Font*& font);

        ~TextBox();
    };

    /// @brief A basic text box, but editable.
    class EditableTextBox : public TextBox {
    private:
        /// @brief The cursor position.
        size_t cursorPosition;

        /// @brief Handles the backspace key.
        void handleBackspace();
        /// @brief Handles the left arrow key.
        void moveCursorLeft();
        /// @brief Handles the right arrow key.
        void moveCursorRight();
        /// @brief Handles the delete key.
        void handleDelete();

        /// @brief Inserts a character.
        void insertCharacter(char c);

        /// @brief The key actions for special keys.
        std::unordered_map<SDL_Keycode, std::function<void()>> keyActions;

        /// @brief The editable state of the text box.
        bool editable;

        /// @brief The class name.
		static inline const std::string CLASS_NAME = "EditableTextBox";
    public:
        EditableTextBox(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit(),
            SDL_Color backgroundColor = SDL_Color(),
            SDL_Color textColor = { 0, 0, 0, 255 },
            TTF_Font* textFont = nullptr,
            HorizontalTextAlign alignX = HorizontalTextAlign::CENTER,
            VerticalTextAlign alignY = VerticalTextAlign::CENTER,
            bool editable = false
        );

        /// @brief Returns the editable state of the text box.
        /// @return True if the text box is editable, false otherwise.
        bool isEditable() const;

        /// @brief Sets the editable state of the text box.
        /// @param val The new editable state.
        void setEditable(bool val);

        /// @brief Resets the text box.
        void reset();

        /// @brief Handles the events.
        /// @param e The event to be handled.
        void handleEvent(const SDL_Event& e) override;

        /// @brief Renders the text box.
        void render() override;

        ~EditableTextBox();
    };

}

#endif /* TEXTBOX_H */