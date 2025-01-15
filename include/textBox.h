#pragma once

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "types.h"

class TextBox {
protected:
    TTF_Font* textFont;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    bool visible;
    TextAlign xAlign;
    TextAlign yAlign;
    SDL_Rect textBoxRect;
    SDL_Color boxColor;
    SDL_Color textColor;

    std::vector<std::string> lines;

    int lineHeight() const;

    std::vector<std::string> splitTextIntoLines(std::string &text, int maxWidth);

public:
    std::string text;

    TextBox(SDL_Rect br, SDL_Color bc, std::string t,
        SDL_Color tc, TTF_Font* tf,
        TextAlign alignX, TextAlign alignY);

    void render(SDL_Renderer* renderer);

    void update(SDL_Renderer* renderer, const char* textToUpdate);

    void adjustTextAlignment(bool isVertical, TextAlign align);

    bool isVisible() const;

    void toggleVisibility(bool value);

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
    EditableTextBox(SDL_Rect buttonRect, SDL_Color backgroundColor,
        SDL_Color textColor, TTF_Font* textFont,
        TextAlign alignX, TextAlign alignY, bool editable);

    void render(SDL_Renderer* renderer);

    void reset();

    void handleEvent(const SDL_Event& e);

    void adjustTextAlignment(bool isVertical, TextAlign align);

    std::string getText() const;

    ~EditableTextBox();
};

#endif /* TEXTBOX_H */