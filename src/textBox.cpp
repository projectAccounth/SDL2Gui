#include "textBox.h"

int GUILib::TextBox::lineHeight() const {
    int fontHeight = TTF_FontHeight(textFont);
    return fontHeight;
}

std::vector<std::string> GUILib::TextBox::splitTextIntoLines(std::string& str, int maxWidth) {
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

void GUILib::TextBox::render() {
    if (!ref) return;
    if (!isVisible() || (parent && !parent->isVisible())) {
        return;
    }

    int padding = 5;
    int maxWidth = objRect.w - padding * 2;

    int totalHeight = static_cast<int>(lines.size()) * lineHeight();
    int startY = objRect.y;
    
    SDL_SetRenderDrawColor(ref, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    SDL_RenderFillRect(ref, &objRect);

    // checking whether the text is empty or not to prevent problematic stuff
    if (text.empty() || !textFont) {
        return;
    }
    
    lines = splitTextIntoLines(text, maxWidth); // For wrapping

    switch (yAlign) {
    case VerticalTextAlign::UP:
        startY = objRect.y + padding;
        break;
    case VerticalTextAlign::CENTER:
        startY = objRect.y + (objRect.h - totalHeight) / 2;
        break;
    case VerticalTextAlign::BOTTOM:
        startY = objRect.y + (objRect.h - totalHeight) - padding;
        break;
    }

    int offsetY = 0;
    SDL_Surface* textSurface = nullptr;

    for (const auto& line : lines) {
        int textWidth = 0, textHeight = 0;
        TTF_SizeText(textFont, line.c_str(), &textWidth, &textHeight);

        int startX = objRect.x;
        switch (xAlign) {
        case HorizontalTextAlign::LEFT:
            startX = objRect.x + padding;
            break;
        case HorizontalTextAlign::CENTER:
            startX = objRect.x + (maxWidth - textWidth) / 2 + padding;
            break;
        case HorizontalTextAlign::RIGHT:
            startX = objRect.x + maxWidth - textWidth - padding;
            break;
        }
        textSurface = TTF_RenderUTF8_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        textTexture = SDL_CreateTextureFromSurface(ref, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };
        SDL_RenderCopy(ref, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void GUILib::TextBox::updateText(const char* textToUpdate) {
    text = textToUpdate;
    render();
}

void GUILib::TextBox::changeFont(TTF_Font*& font) {
    textFont = font;
}

GUILib::TextBox::TextBox(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    SDL_Color boxColor,
    std::string text,
    SDL_Color textColor,
    TTF_Font* textFont,
    HorizontalTextAlign alignX,
    VerticalTextAlign alignY
) :
    GuiObject(parent, renderer, size, position),
    textTexture(nullptr),
    boxColor(boxColor),
    text(std::move(text)),
    textColor(textColor),
    textFont(textFont),
    xAlign(alignX),
    yAlign(alignY) 
{
}

GUILib::TextBox::~TextBox() {
    if (textTexture) SDL_DestroyTexture(textTexture);
}

SDL_Color GUILib::TextBox::getBoxColor() const {
    return boxColor;
}

SDL_Color GUILib::TextBox::getTextColor() const {
    return textColor;
}

std::string GUILib::TextBox::getText() const {
    return text;
}

void GUILib::TextBox::setBoxColor(const SDL_Color& color) {
    boxColor = color;
}

void GUILib::TextBox::setTextColor(const SDL_Color& color) {
    textColor = color;
}

void GUILib::TextBox::setText(const std::string& str) {
    text = str;
}