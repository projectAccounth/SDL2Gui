#include "textBox.h"

void GUILib::EditableTextBox::handleEvent(const SDL_Event& e) {
    GuiObject::handleEvent(e); // Calling superclass function
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

bool GUILib::EditableTextBox::isEditable() const {
    return editable;
}

void GUILib::EditableTextBox::setEditable(bool val) {
    editable = val;
    trigger("onEditableChange");
}

void GUILib::EditableTextBox::render() {
    TextBox::render(); // Call superclass render function
    /*
    if (editable) {
        // Render cursor
        SDL_Rect cursorRect = { objRect.x, objRect.y, 2, objRect.h };
        if (text.size() > 0) {
            TTF_SizeText(textFont, text.substr(0, cursorPosition).c_str(), &cursorRect.w, nullptr);
        }
        SDL_SetRenderDrawColor(ref, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(ref, &cursorRect);
    }
    */
}

void GUILib::TextBox::adjustTextAlignment(
    const HorizontalTextAlign& alignX,
    const VerticalTextAlign& alignY
) {
    xAlign = alignX;
    yAlign = alignY;
    render();
}