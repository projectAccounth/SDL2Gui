#include "textBox.h"

int GUILib::TextBox::lineHeight() const
{
    if (!textFont) return 0;
    return TTF_FontHeight(textFont);
}

std::vector<std::string> GUILib::TextBox::splitTextIntoLines(
    const std::string& str,
    const int& maxWidth) const
{
    std::vector<std::string> outLines;
    std::string currentLine;
    std::string word;

    for (const char& c : str) {
        // cut the line
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

    // last line
    if (!currentLine.empty() || !word.empty()) {
        outLines.push_back(currentLine + word);
    }
    return outLines;
}

void GUILib::TextBox::render()
{
    if (!shouldRender()) return;

    constexpr int padding = 5;
    const int maxWidth = objRect.w - padding * 2;

    // Shouldn't cause a problem
    const int totalHeight = static_cast<int>(lines.size()) * lineHeight();

    // takes ref (the renderer)
    const Reserved::TextureType boxTexture = Reserved::createSolidBoxTexture(ref, boxColor, objRect.w, objRect.h);

    const SDL_Point offsetPoint = getPivotOffsetPoint();

    // Rotated box? maybe later, rotation should be hidden in the meantime
    SDL_RenderCopyEx(ref, boxTexture.get(), nullptr, &objRect, degreeRotation, &offsetPoint, SDL_FLIP_NONE);

    // checking whether the text is empty or not to prevent problematic stuff
    if (text.empty() || !textFont) {
        GuiObject::render();
        return;
    }
    
    lines = splitTextIntoLines(text, maxWidth); // For wrapping (manual handling)

	int startY = objRect.y;
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
    // properly implement rotated text rendering (later)
    renderedLines.clear(); // <- a member variable in TextBox

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

        const SDL_Point linePos = { startX, startY + offsetY };
        renderedLines.push_back({ line, linePos });

        // Text rendering (unchanged)
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ref, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };

        SDL_RenderCopy(ref, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
    GuiObject::render();
}

void GUILib::TextBox::updateText(const char* textToUpdate)
{
    text = textToUpdate;
    render();
}

void GUILib::TextBox::changeFont(TTF_Font*& font)
{
    textFont = font;
}

GUILib::TextBox::TextBox(
    std::shared_ptr<GuiObject> parent,
    SDL_Renderer* renderer,
    const UIUnit& size,
    const UIUnit& position,
    const SDL_Color& boxColor,
    std::string text,
    const SDL_Color& textColor,
    TTF_Font* textFont,
    const HorizontalTextAlign& alignX,
    const VerticalTextAlign& alignY) : GuiObject(parent, renderer, size, position),
                                       boxColor(boxColor),
                                       text(std::move(text)),
                                       textColor(textColor),
                                       textFont(textFont),
                                       xAlign(alignX),
                                       yAlign(alignY) {
}

GUILib::TextBox::TextBox() = default;

GUILib::TextBox::~TextBox() = default;

SDL_Color GUILib::TextBox::getBoxColor() const
{
    return boxColor;
}

SDL_Color GUILib::TextBox::getTextColor() const
{
    return textColor;
}

std::string GUILib::TextBox::getText() const
{
    return text;
}

void GUILib::TextBox::setBoxColor(const SDL_Color& color)
{
    boxColor = color;
}

void GUILib::TextBox::setTextColor(const SDL_Color& color)
{
    textColor = color;
}

void GUILib::TextBox::setText(const std::string& str)
{
    text = str;
}

GUILib::TextBox& GUILib::TextBox::operator=(const TextBox& other) = default;
GUILib::TextBox& GUILib::TextBox::operator=(TextBox&& other) noexcept = default;
GUILib::TextBox::TextBox(TextBox&&) noexcept = default;
GUILib::TextBox::TextBox(const TextBox&) = default;


