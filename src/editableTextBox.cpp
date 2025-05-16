#include "textBox.h"

namespace {
    std::pair<size_t, size_t> getCursorPos(
        const std::vector<std::string>& lines,
        size_t cursorPos, 
        const std::string& fullText) 
    {
        if (lines.empty() || fullText.empty()) return { 0, 0 };
        size_t globalIndex = 0;
        for (size_t i = 0; i < lines.size(); ++i) {
            size_t lineLen = lines[i].size();
            // Is the cursor is within this line?
            if (cursorPos <= globalIndex + lineLen) {
                return { i, cursorPos - globalIndex };
            }
            // Account for the newline character that was split
            globalIndex += lineLen;
            if (globalIndex < fullText.size() && fullText[globalIndex] == '\n') {
                ++globalIndex;
                // Prob in the new line
                if (cursorPos == globalIndex) {
                    return { i + 1, 0 };
                }
            }
        }
        // Fallback at the end
        return { lines.size() - 1, lines.back().size() };
    }

    size_t getGlobalCursorPos(
        const std::vector<std::string>& lines,
        size_t lineIdx, size_t colIdx,
        const std::string& fullText) 
    {
        size_t globalIndex = 0;
        size_t textIdx = 0;
        for (size_t i = 0; i < lines.size(); ++i) {
            if (i == lineIdx) {
                return globalIndex + std::min(colIdx, lines[i].size());
            }
            globalIndex += lines[i].size();
            if (textIdx + lines[i].size() < fullText.size() && fullText[globalIndex] == '\n') {
                ++globalIndex;
            }
            textIdx += lines[i].size();
            if (textIdx < fullText.size() && fullText[textIdx] == '\n') {
                ++textIdx;
            }
        }
        return fullText.size();
    }
}

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
    }
}

void GUILib::EditableTextBox::moveCursorRight()
{
    if (cursorPosition < text.size()) {
        cursorPosition++;
    }
}

void GUILib::EditableTextBox::moveCursorUp() {
    auto [lineIdx, colIdx] = getCursorPos(lines, cursorPosition, text);

    if (lineIdx == 0) return; // First line
    if (preferredColumn == 0) preferredColumn = colIdx;

    size_t newLineIdx = lineIdx - 1;
    size_t newColIdx = std::min(preferredColumn, lines[newLineIdx].size());
    cursorPosition = getGlobalCursorPos(lines, newLineIdx, newColIdx, text);
}

void GUILib::EditableTextBox::moveCursorDown() {
    auto [lineIdx, colIdx] = getCursorPos(lines, cursorPosition, text);

    if (lineIdx + 1 >= lines.size()) return; // Last line
    if (preferredColumn == 0) preferredColumn = colIdx;

    size_t newLineIdx = lineIdx + 1;
    size_t newColIdx = std::min(preferredColumn, lines[newLineIdx].size());
    cursorPosition = getGlobalCursorPos(lines, newLineIdx, newColIdx, text);
}

void GUILib::EditableTextBox::insertCharacter(char c)
{
    // Arguably the worst way to handle it
    if (c == '\n') {
        // Start of the current line
        size_t lineStart = text.rfind('\n', cursorPosition == 0 ? 0 : cursorPosition - 1);
        if (lineStart == std::string::npos) lineStart = 0;
        else lineStart += 1; // Move past the '\n'

        // collect indentation (spaces/tabs) from the current line
        std::string indentation;
        while (lineStart + indentation.size() < text.size()) {
            char ch = text[lineStart + indentation.size()];
            if (ch == ' ' || ch == '\t')
                indentation += ch;
            else
                break;
        }

        // Check the last non-whitespace character before the cursor
        size_t lastCharPos = cursorPosition;
        while (lastCharPos > lineStart && (text[lastCharPos - 1] == ' ' || text[lastCharPos - 1] == '\t'))
            --lastCharPos;
        char lastChar = (lastCharPos > lineStart) ? text[lastCharPos - 1] : '\0';
        
        text.insert(cursorPosition, 1, '\n');
        cursorPosition++;

        // if last non-whitespace char is {, add extra indent
        if (lastChar == '{' && shouldIndent) {
            indentation += std::string(indentationSize, ' ');
        }
        
        text.insert(cursorPosition, indentation);
        cursorPosition += indentation.size();
    }
    // Auto unindent
    else if (c == '}' && shouldIndent) {
        // Find the start of the current line
        size_t lineStart = text.rfind('\n', cursorPosition == 0 ? 0 : cursorPosition - 1);
        if (lineStart == std::string::npos) lineStart = 0;
        else lineStart += 1;

        // If the current line is only indentation, remove one indent before inserting }
        size_t indentEnd = lineStart;
        while (indentEnd < text.size() && (text[indentEnd] == ' ' || text[indentEnd] == '\t'))
            ++indentEnd;
        if (cursorPosition == indentEnd) {
            // Remove one indent (tab or 4 spaces or whatever)
            if (indentEnd > lineStart) {
                if (text[indentEnd - 1] == '\t') {
                    text.erase(indentEnd - 1, 1);
                    cursorPosition--;
                }
                else if (indentEnd - lineStart >= indentationSize &&
                    text.substr(indentEnd - indentationSize, indentationSize) == std::string(indentationSize, ' ')) {
                    text.erase(indentEnd - indentationSize, indentationSize);
                    cursorPosition -= indentationSize;
                }
            }
        }
        text.insert(cursorPosition, 1, c);
        cursorPosition++;
    }
    else {
        text.insert(cursorPosition, 1, c);
        cursorPosition++;
    }
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
    cursorPosition(0),
    indentationSize(0), shouldIndent(false)
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
    keyActions[SDLK_UP] = [this]()
    {
        moveCursorUp();
    };
    keyActions[SDLK_DOWN] = [this]()
    {
        moveCursorDown();
    };
    keyActions[SDLK_DELETE] = [this]()
    {
	    handleDelete();
    };
    keyActions[SDLK_RETURN] = [this]()
    {
	    insertCharacter('\n');
    };
    keyActions[SDLK_TAB] = [this]() {
        for (int i = 1; i <= indentationSize; ++i) insertCharacter(' ');
    };
}

GUILib::EditableTextBox::EditableTextBox():
	editable(false), cursorPosition(0), indentationSize(0), shouldIndent(false)
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
    keyActions[SDLK_UP] = [this]()
    {
        moveCursorUp();
    };
    keyActions[SDLK_DOWN] = [this]()
    {
        moveCursorDown();
    };
    keyActions[SDLK_DELETE] = [this]()
    {
    	handleDelete();
    };
    keyActions[SDLK_RETURN] = [this]()
    {
    	insertCharacter('\n');
    };
    keyActions[SDLK_TAB] = [this]() 
    {
        for (int i = 1; i <= indentationSize; ++i) insertCharacter(' ');
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
    if (!editable) return;

    // Update cursor blinking timer
    auto now = std::chrono::steady_clock::now();
    if (now - lastBlinkTime > std::chrono::milliseconds(500)) {
        cursorVisible = !cursorVisible;
        lastBlinkTime = now;
    }

    if (!cursorVisible || !textFont) return;

    // map global cursor position to line and column
    auto [lineIdx, colIdx] = getCursorPos(lines, cursorPosition, text); // crash??
    if (lineIdx >= renderedLines.size()) return; // WHY DID THAT BREAK

    const auto& [lineText, linePosition] = renderedLines[lineIdx];
    std::string beforeCursor = lineText.substr(0, colIdx);

    int cursorOffsetX = 0;
    TTF_SizeText(textFont, beforeCursor.c_str(), &cursorOffsetX, nullptr);

    int cursorX = linePosition.x + cursorOffsetX;
    int cursorY = linePosition.y;
    int height = TTF_FontHeight(textFont);

    SDL_SetRenderDrawColor(ref, textColor.r, textColor.g, textColor.b, textColor.a);
    SDL_RenderDrawLine(ref, cursorX, cursorY, cursorX, cursorY + height);
}

bool GUILib::EditableTextBox::getShouldIndent() const {
    return shouldIndent;
}

int GUILib::EditableTextBox::getIndentationSize() const {
    return indentationSize;
}

void GUILib::EditableTextBox::setShouldIndent(const bool& val) {
    shouldIndent = val;
}

void GUILib::EditableTextBox::setIndentationSize(const int& val) {
    indentationSize = val;
}