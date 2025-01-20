#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "types.h"
#include "guiobject.h"

class TextButton : public GuiObject {
protected:
    static int nextId;

    bool isClicked(int x, int y);

    void checkHover(int mouseX, int mouseY);

    SDL_Color buttonColor;

    SDL_Color textColor; // color for TextButton
    SDL_Texture* textTexture;
    SDL_Color hoverColor; // color for hovering

    TTF_Font* textFont; // font for the text in the TextButton

    TextAlign xAlign, yAlign;
    std::function<void()> buttonAction;
    std::function<void()> hoverAction;

    bool hovered;

    // The "ID" of the button, can be used for querying.
    int id;
public:
    std::string text; // text for the TextButton

    TextButton();
    TextButton(
        UIUnit size,
        UIUnit position,
        std::optional<GuiObject*> parent,
        SDL_Renderer*& renderer,
        SDL_Color buttonColor,
        SDL_Color hoverColor,
        SDL_Color textColor = {0, 0, 0, 255},
        std::string text = "",
        TTF_Font* textFont = nullptr,
        TextAlign alignX = CENTER,
        TextAlign alignY = CENTER
    );

    // Preloading text (must be called before rendering)
    void loadText(SDL_Renderer* renderer);

    // Basically just rendering the button on the specified renderer.
    void render(SDL_Renderer* renderer) override;

    // Setting the action for the button.
    void setAction(std::function<void()> actionFunction);

    // Setting the function responsible for hovering.
    void setHoverAction(std::function<void()> actionFunction);

    // Handle all clicking and hovering events. Put in the PollEvent loop.
    void handleEvents(SDL_Event& event);

    // Alternative to button.active = value : bool;
    void toggleActive(bool value);

    int getId() const;

    void changeTextColor(const SDL_Color& color, SDL_Renderer* renderer);

    void changeHoverColor(const SDL_Color& color, SDL_Renderer* renderer);

    void changeButtonColor(const SDL_Color& color, SDL_Renderer* renderer);

    void changeFont(TTF_Font* font, SDL_Renderer* renderer);

    ~TextButton();
};

class ImageButton : public GuiObject {
protected:
    SDL_Texture* buttonTexture;
    SDL_Texture* hoverTexture; // texture for hovering, set hoverTexture to buttonTexture if you don't want hover.
    static int nextId;

    bool isClicked(int x, int y);

    void checkHover(int mouseX, int mouseY);
    int id;

    bool hovered;
    
    std::function<void()> hoverAction;
    std::function<void()> buttonAction;

    std::string defaultImgPath;
    std::string hoverImgPath;
public:

    ImageButton();
    ImageButton(
        UIUnit size,
        UIUnit position,
        std::optional<GuiObject*> parent,
        SDL_Renderer*& renderer,
        std::string defaultImageFilePath,
        std::string hoverImageFilePath
    );

    void initialize(SDL_Renderer* renderer);

    // Renders the button.
    void render(SDL_Renderer* renderer) override;

    // Set an action (void function) for the button.
    void setAction(std::function<void()> actionFunction);

    // Setting the function responsible for hovering.
    void setHoverAction(std::function<void()> actionFunction);

    // Handles clicking and hovering. Put in the PollEvent loop.
    void handleEvents(SDL_Event& event);

    int getId() const;

    void updateHoverImgPath(const char* updatedPath, SDL_Renderer* renderer);

    void updateDefaultImgPath(const char* updatedPath, SDL_Renderer* renderer);

    ~ImageButton();
};

class CheckBox : public TextButton {
private:
    char boxSymbol;
public:
    bool isChecked;

    CheckBox();
    CheckBox(
        TTF_Font* textFont,
        SDL_Color boxColor,
        SDL_Color textColor,
        std::optional<GuiObject*> parent,
        SDL_Renderer*& renderer,
        UIUnit position = { 0, 0, false }, UIUnit size = { 10, 10, false },
        char symbol = 'X'
    );

    void setAction(std::function<void()> buttonAction) = delete;

    void setHoverAction(std::function<void()> buttonAction) = delete;

    void changeSymbol(char symbol);

    void handleEvents(SDL_Event& event);
};


#endif /* BUTTON_H */

