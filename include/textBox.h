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
    SDL_Color boxColor;
    SDL_Color textColor;

    std::vector<std::string> lines;

    int lineHeight() const;

    std::vector<std::string> splitTextIntoLines(std::string &text, int maxWidth);

public:
    std::string text;

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

    void render(SDL_Renderer* renderer) override;

    void update(SDL_Renderer* renderer, const char* textToUpdate);

    void adjustTextAlignment(bool isVertical, TextAlign align);

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

    void render(SDL_Renderer* renderer) override;

    void reset();

    void handleEvent(const SDL_Event& e);

    void adjustTextAlignment(bool isVertical, TextAlign align);

    std::string getText() const;

    ~EditableTextBox();
};

#endif /* TEXTBOX_H */