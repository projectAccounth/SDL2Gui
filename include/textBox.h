#pragma once

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "types.h"
#include "guiobject.h"


namespace GUILib {

    // A basic text box.
    // Can be used for texts, just remember to set the background colors' alpha channel to 0.
    class TextBox : public GuiObject {
    protected:
        TTF_Font* textFont;
        SDL_Texture* textTexture;
        HorizontalTextAlign xAlign;
        VerticalTextAlign yAlign;

        std::vector<std::string> lines;

        int lineHeight() const;

        std::vector<std::string> splitTextIntoLines(std::string& text, int maxWidth);

        std::string text;
        SDL_Color boxColor;
        SDL_Color textColor;
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

        void render() override;

        void updateText(const char* textToUpdate);

        inline void adjustTextAlignment(
            const HorizontalTextAlign& alignX,
            const VerticalTextAlign& alignY
        ) {
            xAlign = alignX;
            yAlign = alignY;
        }

        inline SDL_Color getBoxColor() const { return boxColor; }
        inline SDL_Color getTextColor() const { return textColor; }
        inline std::string getText() const { return text; }

        inline void setBoxColor(const SDL_Color& color) {
            boxColor = color;
        }
        inline void setTextColor(const SDL_Color& color) {
            textColor = color;
        }
        inline void setText(const std::string& str) {
            text = str;
        }

        void changeFont(TTF_Font*& font);

        ~TextBox();
    };

    // A basic text box, but editable.
    class EditableTextBox : public TextBox {
    private:
        size_t cursorPosition;

        void handleBackspace();
        void moveCursorLeft();
        void moveCursorRight();
        void handleDelete();

        void insertCharacter(char c);

        std::unordered_map<SDL_Keycode, std::function<void()>> keyActions;
        bool editable;
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

        inline bool isEditable() const { return editable; }
        inline void setEditable(bool val) { 
            editable = val;
            trigger("onEditableChange", val);
        }

        void reset();

        void handleEvents(const SDL_Event& e);

        ~EditableTextBox();
    };

}

#endif /* TEXTBOX_H */