#include "../include/textBox.h"

void GUILib::EditableTextBox::render() {
    if (!ref)
        return;

    int padding = 0;
    int maxWidth = objRect.w - padding * 2;

    int totalHeight = static_cast<int>(lines.size()) * lineHeight();
    int startY = objRect.y;

    if (!isVisible() || (parent && !parent->isVisible())) {
        return;
    }

    SDL_SetRenderDrawColor(ref, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    SDL_RenderFillRect(ref, &objRect);

    // checking whether the text is empty or not to prevent problematic stuff
    if (text.empty()) {
        return;
    }

    lines = splitTextIntoLines(text, maxWidth);

    // LEFT = up, CENTER = center; RIGHT = down.
    int offsetY = 0;

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

    for (const auto& line : lines) {
        int textWidth = 0, textHeight = 0;
        TTF_SizeText(textFont, line.c_str(), &textWidth, &textHeight);

        int startX = objRect.x;
        switch (xAlign) {
        case HorizontalTextAlign::LEFT:
            startX = objRect.x + padding;
            break;
        case HorizontalTextAlign::CENTER:
            startX = objRect.x + (maxWidth - textWidth) / 2;
            break;
        case HorizontalTextAlign::RIGHT:
            startX = objRect.x + maxWidth - textWidth - padding;
            break;
        }

        textSurface = TTF_RenderText_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        textTexture = SDL_CreateTextureFromSurface(ref, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };
        SDL_RenderCopy(ref, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void GUILib::EditableTextBox::handleEvents(const SDL_Event& e) {
    handleEvent(e);
    if (!editable) return;
    if (e.type == SDL_KEYDOWN) {
        auto it = keyActions.find(e.key.keysym.sym);
        if (it != keyActions.end()) {
            it->second(); // Call the corresponding action
        }
    }
    else if (e.type == SDL_TEXTINPUT) {
        insertCharacter(e.text.text[0]); // Insert the input character
        trigger("onKeyInput", e.text.text[0]);
    }
}

void GUILib::EditableTextBox::handleBackspace() {
    if (cursorPosition > 0) {
        text.erase(cursorPosition - 1, 1);
        cursorPosition--;
    }
}

void GUILib::EditableTextBox::handleDelete() {
    if (cursorPosition < text.size()) {
        text.erase(cursorPosition, 1);
    }
}

void GUILib::EditableTextBox::moveCursorLeft() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}

void GUILib::EditableTextBox::moveCursorRight() {
    if (cursorPosition < text.size()) {
        cursorPosition++;
    }
}

void GUILib::EditableTextBox::insertCharacter(char c) {
    text.insert(cursorPosition, 1, c);
    cursorPosition++;
}

void GUILib::EditableTextBox::reset() {
    text.clear();
    cursorPosition = 0;
}

GUILib::EditableTextBox::EditableTextBox(
    GuiObject* parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    SDL_Color backgroundColor,
    SDL_Color textColor,
    TTF_Font* textFont,
    HorizontalTextAlign alignX,
    VerticalTextAlign alignY,
    bool editable
):
    TextBox(
        parent, renderer,
        size, position,
        backgroundColor,
        "",
        textColor,
        textFont,
        alignX, alignY
    ),
    editable(editable),
    cursorPosition(0) {
    // Define actions for specific keys
    keyActions[SDLK_BACKSPACE] = [this]() { handleBackspace(); };
    keyActions[SDLK_LEFT] = [this]() { moveCursorLeft(); };
    keyActions[SDLK_RIGHT] = [this]() { moveCursorRight(); };
    keyActions[SDLK_DELETE] = [this]() { handleDelete(); };
    keyActions[SDLK_RETURN] = [this]() { insertCharacter('\n'); };
}

GUILib::EditableTextBox::~EditableTextBox() {
    if (textTexture)
        SDL_DestroyTexture(textTexture);
}