#include "button.h"
#include "types.h"

int GUILib::TextButton::nextId = 0;

void GUILib::TextButton::initialize(SDL_Renderer*& renderer) {
    if (!renderer)
        return;

    if ((text.empty() || !textFont) && !textTexture) {
        textTexture = nullptr;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        return;
    }
    if (textTexture != nullptr)
        SDL_DestroyTexture(textTexture);

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(textFont, text.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Cannot create surface for text, error: " << TTF_GetError() << "\n";
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (textTexture == nullptr) {
        std::cerr << "Cannot create text texture. Error: " << TTF_GetError() << "\n";
        return;
    }
}

void GUILib::TextButton::render() {
    if (!isVisible() || (parent && !parent->isVisible())) {
        return;
    }
    // set the color to draw for the button and also set the settings to render the button
    SDL_Color drawColor = hovered ? hoverColor : buttonColor;
    // the color of the button will be the color of the hovered button when disabled (by default)
    if (!active) {
        drawColor = hoverColor;
    }
    SDL_SetRenderDrawColor(ref, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
    SDL_RenderFillRect(ref, &objRect);
    if (textTexture && textFont) {
        int textWidth = 0, textHeight = 0;

        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

        SDL_Rect textRect;

        // align the text
        switch (xAlign) {
        case HorizontalTextAlign::LEFT:
            textRect.x = objRect.x + 5;
            break;
        case HorizontalTextAlign::CENTER:
            textRect.x = objRect.x + (objRect.w - textWidth) / 2;
            break;
        case HorizontalTextAlign::RIGHT:
            textRect.x = objRect.x + (objRect.w - textWidth) - 5;
            break;
        }

        switch (yAlign) {
        case VerticalTextAlign::UP:
            textRect.y = objRect.y + 5;
            break;
        case VerticalTextAlign::CENTER:
            textRect.y = objRect.y + (objRect.h - textHeight) / 2;
            break;
        case VerticalTextAlign::BOTTOM:
            textRect.y = objRect.y + (objRect.h - textHeight) - 5;
            break;
        }

        textRect.w = textWidth; textRect.h = textHeight;

        SDL_RenderCopy(ref, textTexture, nullptr, &textRect);
    }
}

int GUILib::TextButton::getId() const {
    return id;
}

void GUILib::TextButton::changeTextColor(const SDL_Color& color) {
    if (
        textColor.r == color.r &&
        textColor.g == color.g &&
        textColor.b == color.b &&
        textColor.a == color.a
        )
        return;
    textColor = color;
    initialize(ref);
}

void GUILib::TextButton::changeHoverColor(const SDL_Color& color) {
    if (
        hoverColor.r == color.r &&
        hoverColor.g == color.g &&
        hoverColor.b == color.b &&
        hoverColor.a == color.a
        )
        return;
    hoverColor = color;
    initialize(ref);
}

void GUILib::TextButton::changeButtonColor(const SDL_Color& color) {
    if (
        buttonColor.r == color.r &&
        buttonColor.g == color.g &&
        buttonColor.b == color.b &&
        buttonColor.a == color.a
        )
        return;
    buttonColor = color;
    initialize(ref);
}

void GUILib::TextButton::changeFont(TTF_Font* font) {
    if (this->textFont == font) return;
    textFont = font;
    initialize(ref);
}

GUILib::TextButton::TextButton(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    SDL_Color buttonColor,
    SDL_Color hoverColor,
    SDL_Color textColor,
    std::string text,
    TTF_Font* textFont,
    HorizontalTextAlign alignX,
    VerticalTextAlign alignY
):
    Button(parent, renderer, size, position),
    buttonColor(buttonColor),
    textColor(textColor),
    textFont(textFont),
    text(std::move(text)),
    xAlign(alignX),
    yAlign(alignY),
    textTexture(nullptr),
    hoverColor(hoverColor),
    id(nextId++)
{
    initialize(ref);
}

GUILib::TextButton::TextButton():
    Button(),
    buttonColor(SDL_Color()),
    textColor(SDL_Color()),
    textFont(nullptr),
    text(""),
    xAlign(HorizontalTextAlign::CENTER),
    yAlign(VerticalTextAlign::CENTER),
    textTexture(nullptr),
    hoverColor(SDL_Color()),
    id(nextId++)
{}

GUILib::TextButton::~TextButton() {
    if (textTexture)
        SDL_DestroyTexture(textTexture);
}



