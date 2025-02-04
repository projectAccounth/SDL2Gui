#pragma once

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "types.h"
#include "guiobject.h"

class TextBox : public GuiObject {
protected:
    TTF_Font* textFont;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    TextAlign xAlign;
    TextAlign yAlign;

    std::vector<std::string> lines;

    int lineHeight() const;

    std::vector<std::string> splitTextIntoLines(std::string &text, int maxWidth);

public:
    std::string text;
    SDL_Color boxColor;
    SDL_Color textColor;
    TextBox(
        UIUnit size,
        UIUnit position,
        std::optional<GuiObject*> parent,
        SDL_Renderer*& renderer,
        SDL_Color boxColor,
        std::string text = "",
        SDL_Color textColor = { 0, 0, 0, 255 },
        TTF_Font* textFont = nullptr,
        TextAlign alignX = CENTER,
        TextAlign alignY = CENTER
    );

    void render() override;

    void updateText(const char* textToUpdate);

    void adjustTextAlignment(bool isVertical, TextAlign align);

    void changeFont(TTF_Font*& font);

    ~TextBox();
};

class EditableTextBox : public TextBox {
private:
    size_t cursorPosition;
    std::string text;

    void handleBackspace();
    void moveCursorLeft();
    void moveCursorRight();
    void handleDelete();

    void insertCharacter(char c);

    std::unordered_map<SDL_Keycode, std::function<void()>> keyActions;
public:
    bool editable;
    EditableTextBox(
        UIUnit size,
        UIUnit position,
        std::optional<GuiObject*> parent,
        SDL_Renderer*& renderer,
        SDL_Color backgroundColor,
        SDL_Color textColor = { 0, 0, 0, 255 },
        TTF_Font* textFont = nullptr,
        TextAlign alignX = CENTER,
        TextAlign alignY = CENTER,
        bool editable = false
    );

    void render() override;

    void reset();

    void handleEvents(const SDL_Event& e);

    void adjustTextAlignment(bool isVertical, TextAlign align);

    std::string getText() const;

    void changeFont(TTF_Font*& font);

    ~EditableTextBox();
};

#endif /* TEXTBOX_H */