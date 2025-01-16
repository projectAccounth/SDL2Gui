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
    TextButton(int x, int y, int w, int h,
        SDL_Color buttonColor, std::string text,
        SDL_Color textColor, SDL_Color hoverColor, TTF_Font* textFont,
        TextAlign alignX = CENTER, TextAlign alignY = CENTER);

    // Preloading text (must be called before rendering)
    void loadText(SDL_Renderer* renderer);

    // Basically just rendering the button on the specified renderer.
    void render(SDL_Renderer* renderer);

    // Setting the action for the button.
    void setAction(std::function<void()> actionFunction);

    // Setting the function responsible for hovering.
    void setHoverAction(std::function<void()> actionFunction);

    // Returns the visibility of the button.
    bool isVisible() const;

    // Alternative to button.visible = value : bool;
    void toggleVisiblility(bool value);

    // Handle all clicking and hovering events. Put in the PollEvent loop.
    void handleEvents(SDL_Event& event);

    // Alternative to button.active = value : bool;
    void toggleActive(bool value);

    int getId() const;

    void changeTextColor(const SDL_Color& color, SDL_Renderer* renderer);

    void changeHoverColor(const SDL_Color& color, SDL_Renderer* renderer);

    void changeButtonColor(const SDL_Color& color, SDL_Renderer* renderer);

    void changeFont(TTF_Font* font, SDL_Renderer* renderer);

    ~TextButton() {}
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
    ImageButton(int x, int y, int w, int h, std::string defaultImageFilePath, std::string hoverImageFilePath);

    void initialize(SDL_Renderer* renderer);

    // Renders the button.
    void render(SDL_Renderer* renderer);

    // Set an action (void function) for the button.
    void setAction(std::function<void()> actionFunction);

    // Setting the function responsible for hovering.
    void setHoverAction(std::function<void()> actionFunction);

    // Returns the visibility.
    bool isVisible() const;

    // Alternative to directly assigning button.visible.
    void toggleVisiblility(bool value);

    // Handles clicking and hovering. Put in the PollEvent loop.
    void handleEvents(SDL_Event& event);

    // Alternative to directly assigning button.active.
    void toggleActive(bool value);

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
        int x = 0, int y = 0, int w = 10, int h = 10,
        char symbol = 'X'
    );

    void changeSymbol(char symbol);

    void handleEvents(SDL_Event& event);
};


#endif /* BUTTON_H */

