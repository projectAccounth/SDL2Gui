#include "textBox.h"

void GUILib::EditableTextBox::handleEvent(const SDL_Event& e)
{
    TextBox::handleEvent(e); // Calling superclass function
    if (!editable) return;
    cursorPosition = std::clamp(cursorPosition, size_t(0), text.size());
    if (e.type == SDL_KEYDOWN) {
        if (auto it = keyActions.find(e.key.keysym.sym); it != keyActions.end()) {
            it->second(); // Call the corresponding action
            trigger("onSpecialKeyInput");
        }
    }
    else if (e.type == SDL_TEXTINPUT) {
        insertCharacter(e.text.text[0]); // Insert the input character
        trigger("onKeyInput", e.text.text[0]);
    }
}

void GUILib::EditableTextBox::handleBackspace()
{
    if (cursorPosition > 0) {
	    text.erase(cursorPosition - 1, 1);
	    cursorPosition--;
    }
}

void GUILib::EditableTextBox::handleDelete()
{
    if (cursorPosition < text.size()) {
        text.erase(cursorPosition, 1);
    }
}

void GUILib::EditableTextBox::moveCursorLeft()
{
    if (cursorPosition > 0) {
        cursorPosition--;
        std::cout << "Moved cursor left;\n";
    }
}

void GUILib::EditableTextBox::moveCursorRight()
{
    if (cursorPosition < text.size()) {
        cursorPosition++;
        std::cout << "Moved cursor right;\n";
    }
}

void GUILib::EditableTextBox::insertCharacter(char c)
{
    text.insert(cursorPosition, 1, c);
    cursorPosition++;
}

void GUILib::EditableTextBox::reset()
{
    text.clear();
    cursorPosition = 0;
}

GUILib::EditableTextBox::EditableTextBox(
    std::shared_ptr<GuiObject> parent,
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
    cursorPosition(0)
{
    // Define actions for specific keys
    keyActions[SDLK_BACKSPACE] = [this]()
    {
	    handleBackspace();
    };
    keyActions[SDLK_LEFT] = [this]()
    {
	    moveCursorLeft();
    };
    keyActions[SDLK_RIGHT] = [this]()
    {
	    moveCursorRight();
    };
    keyActions[SDLK_DELETE] = [this]()
    {
	    handleDelete();
    };
    keyActions[SDLK_RETURN] = [this]()
    {
	    insertCharacter('\n');
    };
}

GUILib::EditableTextBox::EditableTextBox():
	editable(false), cursorPosition(0)
{
    keyActions[SDLK_BACKSPACE] = [this]()
    {
    	handleBackspace();
    };
    keyActions[SDLK_LEFT] = [this]()
    {
    	moveCursorLeft();
    };
    keyActions[SDLK_RIGHT] = [this]()
    {
    	moveCursorRight();
    };
    keyActions[SDLK_DELETE] = [this]()
    {
    	handleDelete();
    };
    keyActions[SDLK_RETURN] = [this]()
    {
    	insertCharacter('\n');
    };
}


GUILib::EditableTextBox::~EditableTextBox() = default;

bool GUILib::EditableTextBox::isEditable() const
{
    return editable;
}

void GUILib::EditableTextBox::setEditable(bool val)
{
    editable = val;
    trigger("onEditableChange");
}

void GUILib::EditableTextBox::render()
{
    TextBox::render();
    if (editable) {
        // Cursor blinking
        auto now = std::chrono::steady_clock::now();
        if (now - lastBlinkTime > std::chrono::milliseconds(500)) {
            cursorVisible = !cursorVisible;
            lastBlinkTime = now;
        }

        if (cursorVisible && textFont) {
            const auto& rendered = renderedLines;

            size_t charCounter = 0;
            for (const auto& line : rendered) {
                const size_t lineLen = line.text.size();

                if (cursorPosition >= charCounter && cursorPosition <= charCounter + lineLen) {
                    const size_t localIndex = cursorPosition - charCounter;
                    const std::string beforeCursor = line.text.substr(0, localIndex);

                    int cursorOffsetX = 0;
                    TTF_SizeText(textFont, beforeCursor.c_str(), &cursorOffsetX, nullptr);

                    int cursorX = line.position.x + cursorOffsetX;
                    int cursorY = line.position.y;
                    int height = TTF_FontHeight(textFont);

                    SDL_SetRenderDrawColor(ref, 0, 0, 0, 255); // Black
                    SDL_RenderDrawLine(ref, cursorX, cursorY, cursorX, cursorY + height);

                    break;
                }

                charCounter += lineLen;
            }
        }
    }
}

void GUILib::TextBox::adjustTextAlignment(
    const HorizontalTextAlign& alignX,
    const VerticalTextAlign& alignY
)
{
    xAlign = alignX;
    yAlign = alignY;
    this->render();
}