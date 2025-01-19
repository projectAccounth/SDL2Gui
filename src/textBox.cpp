#include "textBox.h"

int TextBox::lineHeight() const {
    int fontHeight = TTF_FontHeight(textFont);
    return fontHeight;
}

std::vector<std::string> TextBox::splitTextIntoLines(std::string& str, int maxWidth) {
    std::vector<std::string> outLines;
    std::string currentLine;
    std::string word;

    for (char c : str) {
        if (c == ' ' || c == '\n') {
            // line measurement
            int wordWidth = 0, wordHeight = 0;
            TTF_SizeText(textFont, (currentLine + word).c_str(), &wordWidth, &wordHeight);

            if (wordWidth > maxWidth) {
                // wrap the text
                outLines.push_back(currentLine);
                currentLine = word;
            }
            else {
                currentLine += word;
            }

            if (c == '\n') {
                outLines.push_back(currentLine);
                currentLine.clear();
            }
            else {
                currentLine += ' ';
            }
            word.clear();
        }
        else {
            word += c;
        }
    }

    // last line!!
    if (!currentLine.empty() || !word.empty()) {
        outLines.push_back(currentLine + word);
    }
    return outLines;
}

void TextBox::render(SDL_Renderer* renderer) {
    int padding = 5;
    int maxWidth = objRect.w - padding * 2;

    int totalHeight = static_cast<int>(lines.size()) * lineHeight();
    int startY = objRect.y;
    if (!isVisible()) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    SDL_RenderFillRect(renderer, &objRect);

    // checking whether the text is empty or not to prevent problematic stuff
    if (text.empty() || !textFont || !textTexture) {
        return;
    }
    
    lines = splitTextIntoLines(text, maxWidth);

    // LEFT = up, CENTER = center; RIGHT = down.
    switch (yAlign) {
    case LEFT:
        startY = objRect.y + padding;
        break;
    case CENTER:
        startY = objRect.y + (objRect.h - totalHeight) / 2;
        break;
    case RIGHT:
        startY = objRect.y + (objRect.h - totalHeight) - padding;
        break;
    }

    int offsetY = 0;

    for (const auto& line : lines) {
        int textWidth = 0, textHeight = 0;
        TTF_SizeText(textFont, line.c_str(), &textWidth, &textHeight);

        int startX = objRect.x;
        switch (xAlign) {
        case LEFT:
            startX = objRect.x + padding;
            break;
        case CENTER:
            startX = objRect.x + (maxWidth - textWidth) / 2;
            break;
        case RIGHT:
            startX = objRect.x + maxWidth - textWidth - padding;
            break;
        }
        textSurface = TTF_RenderUTF8_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void TextBox::update(SDL_Renderer* renderer, const char* textToUpdate) {
    text = textToUpdate;
    render(renderer);
}

void TextBox::adjustTextAlignment(bool isVertical, TextAlign align) {
    if (isVertical) {
        yAlign = align;
        return;
    }
    xAlign = align;
}

TextBox::TextBox(
    UIUnit size,
    UIUnit position,
    std::optional<GuiObject*> parent,
    SDL_Renderer* renderer,
    SDL_Color boxColor,
    std::string text,
    SDL_Color textColor,
    TTF_Font* textFont,
    TextAlign alignX,
    TextAlign alignY
) :
    GuiObject(size, position, parent, renderer),
    textTexture(nullptr),
    textSurface(nullptr),
    boxColor(boxColor),
    text(text),
    textColor(textColor),
    textFont(textFont),
    xAlign(alignX),
    yAlign(alignY) {}

TextBox::~TextBox() {
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}