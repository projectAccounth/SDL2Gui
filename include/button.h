#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "types.h"
#include "guiobject.h"
#include "textBox.h"

namespace GUILib {

    // Abstract, practically useless button class.
    // Serves as a base for buttons.
    class Button : public GuiObject {
    protected:
        std::function<void()> buttonAction;
        std::function<void()> hoverAction;

        bool isClicked(int x, int y);
        void checkHover(int mouseX, int mouseY);

        bool hovered;
    public:
        Button();
        Button(
            GuiObject* parent,
            SDL_Renderer*& renderer,
            UIUnit size = UIUnit(),
            UIUnit position = UIUnit()
        );

        // @deprecated Migrate to the event system.
        // Setting the function responsible for clicking.
        inline void setAction(const std::function<void()>& actionFunction) {
            buttonAction = actionFunction;
        }
        // @deprecated Migrate to the event system.
        // Setting the function responsible for hovering.
        inline void setHoverAction(const std::function<void()>& actionFunction) {
            buttonAction = actionFunction;
        }

        void handleEvents(SDL_Event& e);

        inline bool isHovered() const { return hovered; }
    };

    // A basic text button for anything.
    class TextButton : public Button {
    protected:
        static int nextId;
        SDL_Texture* textTexture;

        TTF_Font* textFont; // font for the text in the TextButton

        HorizontalTextAlign xAlign;
        VerticalTextAlign yAlign;

        std::string text; // text
        SDL_Color buttonColor; // buttonColor
        SDL_Color textColor; // textColor
        SDL_Color hoverColor; // hoverColor

        // The "ID" of the button, can be used for querying.
        int id;
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

        // Preloading text (must be called before rendering)
        void initialize(SDL_Renderer*& renderer);

        // Basically just rendering the button on the specified renderer.
        void render() override;

        int getId() const;

        // Changes text color.
        void changeTextColor(
            const SDL_Color& color
        );

        // Changes hover color.
        void changeHoverColor(
            const SDL_Color& color
        );

        // Changes button color.
        void changeButtonColor(
            const SDL_Color& color
        );

        inline SDL_Color getTextColor() const { return textColor; }
        inline SDL_Color getHoverColor() const { return hoverColor; }
        inline SDL_Color getButtonColor() const { return buttonColor; }

        inline void setText(
            const std::string& str
        ) { 
            text = str;
            trigger("onTextChange", str);
        }
        inline std::string getText() const { return text; }

        void changeFont(
            TTF_Font* font
        );

        ~TextButton();
    };
    
    // A basic image button wrapper.
    class ImageButton : public Button {
    protected:
        SDL_Texture* buttonTexture;
        SDL_Texture* hoverTexture; // texture for hovering, set hoverTexture to buttonTexture if you don't want hover.
        static int nextId;
        int id;

        std::string defaultImgPath;
        std::string hoverImgPath;
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

        void initialize(
            SDL_Renderer*& renderer
        );

        // Renders the button.
        void render() override;

        int getId() const;

        void updateHoverImgPath(
            const char* updatedPath,
            SDL_Renderer*& renderer
        );

        void updateDefaultImgPath(
            const char* updatedPath,
            SDL_Renderer*& renderer
        );

        ~ImageButton();
    };

    // A basic check box.
    class CheckBox : public TextButton {
    private:
        char boxSymbol;
        bool checked;
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

        inline void toggleChecked() { checked = !checked; }
        inline void toggleChecked(bool val) { checked = val; }
        inline bool isChecked() const { return checked; }

        void setAction(
            const std::function<void()>& buttonAction
        ) = delete;

        void setHoverAction(
            const std::function<void()>& buttonAction
        ) = delete;

        void changeSymbol(const char& symbol);

        inline char getSymbol() const { return boxSymbol; }

        void handleEvents(SDL_Event& event);
    };

}


#endif /* BUTTON_H */

