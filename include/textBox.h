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
        /// @brief The alignment of the text.
        HorizontalTextAlign xAlign;
        /// @brief The alignment of the text.
        VerticalTextAlign yAlign;

        std::pair<int, int> textRenderLocation;

        /// @brief The split lines for rendering.
        std::vector<std::string> lines;

        /// @brief The height of the lines based on the size of it.
        [[nodiscard]] int lineHeight() const;

        /// @brief Splits the text into lines.
        [[nodiscard]] std::vector<std::string> splitTextIntoLines(const std::string& str, const int& maxWidth) const;

        /// @brief Renders the text box.
        std::string text;
        /// @brief The color of the box.
        SDL_Color boxColor;
        /// @brief The color of the text.
        SDL_Color textColor;

        /// @brief A struct that stores information about a rendered line in the text box.
        struct RenderedLine {
            /// @brief The text on that line.
            std::string text; 
            /// @brief Pixel position on screen (x, y)
            SDL_Point position; 
        };

        /// @brief The lines that will be rendered.
        std::vector<RenderedLine> renderedLines;

		TextBox(
            std::shared_ptr<GuiObject> parent,
            SDL_Renderer* renderer,
            const UIUnit& size = UIUnit(),
            const UIUnit& position = UIUnit(),
            const SDL_Color& boxColor = SDL_Color(),
            std::string text = "",
            const SDL_Color& textColor = { 0, 0, 0, 255 }, // rgba
            TTF_Font* textFont = nullptr,
            const HorizontalTextAlign& alignX = HorizontalTextAlign::CENTER,
            const VerticalTextAlign& alignY = VerticalTextAlign::CENTER
        );
    public:
        TextBox();

        class Builder : public GuiObject::Builder<Builder, TextBox> {};

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
        [[nodiscard]] SDL_Color getBoxColor() const;

        /// @brief Returns the text color.
        /// @return The text color.
        [[nodiscard]] SDL_Color getTextColor() const;

        /// @brief Returns the text of the text box.
        /// @return The text of the text box.
        [[nodiscard]] std::string getText() const;

        /// @brief Changes the background color.
        void setBoxColor(const SDL_Color& color);

        /// @brief Changes the text color.
        void setTextColor(const SDL_Color& color);

        /// @brief Changes the text of the text box.
        void setText(const std::string& str);

        /// @brief Changes the font of the text.
        void changeFont(TTF_Font*& font);

        /// @brief Returns the class name of the object.
		/// @return The class name.
		inline std::string getClassName() const override { return "TextBox"; };

        TextBox& operator=(const TextBox&);
        TextBox& operator=(TextBox&&) noexcept;
        TextBox(const TextBox&);
        TextBox(TextBox&&) noexcept;

        ~TextBox() override;
    };

    /// @brief A basic text box, but editable.
    class EditableTextBox final : public TextBox {
    protected:
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


        /// Blinking timer and visibility
        bool cursorVisible = true;
        std::chrono::time_point<std::chrono::steady_clock> lastBlinkTime;

        // For future selection support
        size_t selectionStart = std::string::npos;
        size_t selectionEnd = std::string::npos;

        /// @brief The class name.
    	static inline const std::string CLASS_NAME = "EditableTextBox";

        EditableTextBox(
            std::shared_ptr<GuiObject> parent,
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
    public:
        EditableTextBox();

        /// @brief Returns the editable state of the text box.
        /// @return True if the text box is editable, false otherwise.
        [[nodiscard]] bool isEditable() const;

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

        /// @brief Returns the class name of the object.
		/// @return The class name.
		[[nodiscard]] inline std::string getClassName() const override { return "EditableTextBox"; }

        ~EditableTextBox() override;

        EditableTextBox& operator=(const EditableTextBox&) = default;
        EditableTextBox& operator=(EditableTextBox&&) noexcept = default;
        EditableTextBox(const EditableTextBox&) = default;
        EditableTextBox(EditableTextBox&&) noexcept = default;

        class Builder final : public GuiObject::Builder<Builder, EditableTextBox> {};
    };

}

#endif /* TEXTBOX_H */