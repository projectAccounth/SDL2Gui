#include "button.h"
#include "types.h"

int TextButton::nextId = 0;

void TextButton::loadText(SDL_Renderer* renderer) {
    if ((text.empty() || !textFont) && !textTexture) {
        textTexture = nullptr;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        return;
    }
    if (textTexture != nullptr)
        SDL_DestroyTexture(textTexture);

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(textFont, text.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Cannot create surface for text, error:" << TTF_GetError() << "\n";
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (textTexture == nullptr) {
        std::cerr << "Cannot create text. Error: " << TTF_GetError() << "\n";
        return;
    }
}

void TextButton::render(SDL_Renderer* renderer) {
    if (!isVisible() || (parent && !parent.value()->visible)) {
        return;
    }
    // set the color to draw for the button and also set the settings to render the button
    SDL_Color drawColor = hovered ? hoverColor : buttonColor;
    // the color of the button will be the color of the hovered button when disabled (by default)
    if (!active) {
        drawColor = hoverColor;
    }
    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
    SDL_RenderFillRect(renderer, &objRect);
    if (textTexture && textFont) {
        int textWidth = 0, textHeight = 0;

        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

        SDL_Rect textRect;

        // align the text
        switch (xAlign) {
        case LEFT:
            textRect.x = objRect.x + 5;
            break;
        case CENTER:
            textRect.x = objRect.x + (objRect.w - textWidth) / 2;
            break;
        case RIGHT:
            textRect.x = objRect.x + (objRect.w - textWidth) - 5;
            break;
        }
        // LEFT = up, CENTER = center; RIGHT = down.
        switch (yAlign) {
        case LEFT:
            textRect.y = objRect.y + 5;
            break;
        case CENTER:
            textRect.y = objRect.y + (objRect.h - textHeight) / 2;
            break;
        case RIGHT:
            textRect.y = objRect.y + (objRect.h - textHeight) - 5;
            break;
        }

        textRect.w = textWidth; textRect.h = textHeight;

        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }
}

void TextButton::setAction(std::function<void()> actionFunction) {
    buttonAction = actionFunction;
}

void TextButton::setHoverAction(std::function<void()> actionFunction) {
    hoverAction = actionFunction;
}

bool TextButton::isClicked(int x, int y) {
    return (x > objRect.x &&
        x < (objRect.x + objRect.w) &&
        y > objRect.y &&
        y < (objRect.y + objRect.h));
}

void TextButton::checkHover(int mouseX, int mouseY) {
    hovered = isClicked(mouseX, mouseY);
}

void TextButton::handleEvents(SDL_Event& e) {
    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    handleEvent(e);

    if (!((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) && active && visible)) {
        return;
    }
    if (x > objRect.x && x < (objRect.x + objRect.w) &&
        y > objRect.y && y < (objRect.y + objRect.h)) {
        hovered = true;
        if (hoverAction) hoverAction();
    }
    else {
        hovered = false;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && hovered &&
        e.button.button == SDL_BUTTON_LEFT) {
        if (buttonAction) buttonAction();
    }
}

void TextButton::toggleActive(bool value) {
    active = value;
}

int TextButton::getId() const {
    return id;
}

void TextButton::changeTextColor(const SDL_Color& color, SDL_Renderer* renderer) {
    textColor = color;
    loadText(renderer);

}

void TextButton::changeHoverColor(const SDL_Color& color, SDL_Renderer* renderer) {
    hoverColor = color;
    loadText(renderer);
}

void TextButton::changeButtonColor(const SDL_Color& color, SDL_Renderer* renderer) {
    buttonColor = color;
    loadText(renderer);
}

void TextButton::changeFont(TTF_Font* font, SDL_Renderer* renderer) {
    textFont = font;
    loadText(renderer);
}

TextButton::TextButton(
    UIUnit size,
    UIUnit position,
    std::optional<GuiObject*> parent,
    SDL_Renderer*& renderer,
    SDL_Color buttonColor,
    SDL_Color hoverColor,
    SDL_Color textColor,
    std::string text,
    TTF_Font* textFont,
    TextAlign alignX,
    TextAlign alignY
)
    :
    GuiObject(size, position, parent, renderer),
    buttonColor(buttonColor),
    textColor(textColor),
    textFont(textFont),
    text(text),
    xAlign(alignX),
    yAlign(alignY),
    textTexture(nullptr),
    hovered(false),
    hoverColor(hoverColor),
    id(nextId++)
{
    loadText(ref);
}

TextButton::TextButton():
    GuiObject(),
    buttonColor(SDL_Color()),
    textColor(SDL_Color()),
    textFont(nullptr),
    text(""),
    xAlign(CENTER),
    yAlign(CENTER),
    textTexture(nullptr),
    hovered(false),
    hoverColor(SDL_Color()),
    id(nextId++)
{}

TextButton::~TextButton() {
    SDL_DestroyTexture(textTexture);
}



