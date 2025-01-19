#include "../include/textBox.h"

void EditableTextBox::render(SDL_Renderer* renderer) {
    int padding = 5;
    int maxWidth = objRect.w - padding * 2;

    int totalHeight = static_cast<int>(lines.size()) * lineHeight();
    int startY = objRect.y;

    if (!isVisible() || (parent && !parent.value()->visible)) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    SDL_RenderFillRect(renderer, &objRect);

    // checking whether the text is empty or not to prevent problematic stuff
    if (text.empty()) {
        return;
    }

    lines = splitTextIntoLines(text, maxWidth);

    // LEFT = up, CENTER = center; RIGHT = down.
    int offsetY = 0;

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

        textSurface = TTF_RenderText_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void EditableTextBox::handleEvent(const SDL_Event& e) {
    if (!editable) return;
    if (e.type == SDL_KEYDOWN) {
        auto it = keyActions.find(e.key.keysym.sym);
        if (it != keyActions.end()) {
            it->second(); // Call the corresponding action
        }
    }
    else if (e.type == SDL_TEXTINPUT) {
        insertCharacter(e.text.text[0]); // Insert the input character
    }
}

void EditableTextBox::handleBackspace() {
    if (cursorPosition > 0) {
        text.erase(cursorPosition - 1, 1);
        cursorPosition--;
    }
}

void EditableTextBox::handleDelete() {
    if (cursorPosition < text.size()) {
        text.erase(cursorPosition, 1);
    }
}

void EditableTextBox::moveCursorLeft() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}

void EditableTextBox::moveCursorRight() {
    if (cursorPosition < text.size()) {
        cursorPosition++;
    }
}

void EditableTextBox::insertCharacter(char c) {
    text.insert(cursorPosition, 1, c);
    cursorPosition++;
}

std::string EditableTextBox::getText() const {
    return text;
}

void EditableTextBox::reset() {
    text.clear();
    cursorPosition = 0;
}

void EditableTextBox::adjustTextAlignment(bool isVertical, TextAlign align) {
    if (isVertical) {
        yAlign = align;
        return;
    }
    xAlign = align;
}

EditableTextBox::EditableTextBox(
    UIUnit size,
    UIUnit position,
    std::optional<GuiObject*> parent,
    SDL_Renderer* renderer,
    SDL_Color backgroundColor,
    SDL_Color textColor,
    TTF_Font* textFont,
    TextAlign alignX,
    TextAlign alignY,
    bool editable
):
    TextBox(size, position, parent, renderer, backgroundColor, "", textColor, textFont, alignX, alignY),
    editable(editable),
    text(""), cursorPosition(0) {
    // Define actions for specific keys
    keyActions[SDLK_BACKSPACE] = [this]() { handleBackspace(); };
    keyActions[SDLK_LEFT] = [this]() { moveCursorLeft(); };
    keyActions[SDLK_RIGHT] = [this]() { moveCursorRight(); };
    keyActions[SDLK_DELETE] = [this]() { handleDelete(); };
    keyActions[SDLK_RETURN] = [this]() { insertCharacter('\n'); };
}

EditableTextBox::~EditableTextBox() {
    SDL_DestroyTexture(textTexture);
}