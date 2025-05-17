#include "ComboBox.h"

void GUILib::ComboBox::initialize(SDL_Renderer* renderer) 
{
    Frame::initialize(renderer);
    for (const auto& child : children) child->setParent(nullptr);
    optionBox = GUILib::ScrollingFrame::Builder()
        .setRenderer(renderer)
        .setActive(true)
        .setVisible(false)
        .setPosition({ 0, 1, true })
        .setSize({ 1, static_cast<double>(optionBoxLength), true })
        .build();

    mainButton = GUILib::TextButton::Builder()
        .setRenderer(renderer)
        .setParent(shared_from_this())
        .setActive(this->active)
        .setVisible(this->visible)
        .setPosition({ 0, 0, true })
        .setSize({ 1, 1, true })
        .build();

    optionBox->setParent(mainButton);

    mainButton->on("onClick", std::function([this](int, int) {
        isOptionBoxShown = !isOptionBoxShown;
    }));

    addOption("");
}

void GUILib::ComboBox::addOption(const std::string& optionName) 
{
    auto option = GUILib::TextButton::Builder()
        .setParent(optionBox)
        .setActive(true)
        .setVisible(true)
        .setPosition({})
        .setSize({})
        .build();

    option->setText(optionName.empty() ? "None" : optionName);
    option->initialize(ref);
    option->on("onClick", std::function([this, option, optionName](int, int) {
        if (optionName.empty()) {
            selectedOption = "";
            isOptionBoxShown = false;
            return;
        }
        selectedOption = option->getText();
        isOptionBoxShown = false;
    }));

    options.push_back(option);
}

void GUILib::ComboBox::removeOption(size_t index) 
{
    if (index >= options.size()) return;

    if (options[index]) {
        options[index]->setParent(nullptr);
    }

    options.erase(options.begin() + index);

    for (size_t i = 0; i < options.size(); ++i) {
        options[i]->move({ 0, static_cast<double>(i) / optionBoxLength, true });
    }
}

void GUILib::ComboBox::removeOptionByName(const std::string& text) 
{
    for (size_t i = 0; i < options.size(); ++i) {
        if (options[i] && options[i]->getText() == text) {
            removeOption(i);
            break;
        }
    }
}

void GUILib::ComboBox::handleEvent(const SDL_Event& e) 
{
    Frame::handleEvent(e);

    const SDL_Rect mainButtonRect = mainButton->getRect();
    const double scrollbarWidth = 0.05 * mainButtonRect.w;
    
    optionBox->resize({ mainButtonRect.w + scrollbarWidth, 0.0 + optionBoxLength * mainButtonRect.h, false });
    optionBox->setContentSize({ 0.0 + mainButtonRect.w, static_cast<double>(options.size()) * mainButtonRect.h, false });
    optionBox->setFrameColor(optionBoxColor);

    optionBox->setVisible(isOptionBoxShown);
    optionBox->setActive(true);

    mainButton->setActive(this->active);
    mainButton->setVisible(this->visible);
    mainButton->setText(selectedOption.empty() ? "Select an option" : selectedOption);
    mainButton->changeButtonColor(buttonColor);
    mainButton->changeHoverColor({ 
        static_cast<Uint8>(buttonColor.r - 22), 
        static_cast<Uint8>(buttonColor.r - 22), 
        static_cast<Uint8>(buttonColor.r - 22), 
        buttonColor.a 
    });
    mainButton->changeFont(textFont);

    for (size_t i = 0; i < options.size(); ++i) {
        auto& button = options[i];
        button->changeButtonColor(buttonColor);
        button->changeFont(textFont);
        button->resize({ 0.0 + mainButtonRect.w, 0.0 + mainButtonRect.h, false });
        button->move({ 0, static_cast<double>(i) * mainButtonRect.h, false });
        button->changeFont(textFont);
        button->setParent(optionBox);
        button->changeHoverColor({
            static_cast<Uint8>(buttonColor.r - 22),
            static_cast<Uint8>(buttonColor.r - 22),
            static_cast<Uint8>(buttonColor.r - 22),
            buttonColor.a
        });
    }
}

[[nodiscard]] int GUILib::ComboBox::getOptionBoxLength() const { return optionBoxLength; }
void GUILib::ComboBox::setOptionBoxLength(const int& val) { optionBoxLength = val; }

[[nodiscard]] bool GUILib::ComboBox::getOptionBoxVisibility() const { return isOptionBoxShown; }
void GUILib::ComboBox::setOptionBoxVisibility(const bool& val) { isOptionBoxShown = val; }

[[nodiscard]] SDL_Color GUILib::ComboBox::getOptionBoxColor() const { return optionBoxColor; }
void GUILib::ComboBox::setOptionBoxColor(const SDL_Color& color) {
    optionBoxColor = color;
}

[[nodiscard]] SDL_Color GUILib::ComboBox::getButtonColor() const { return buttonColor; }
void GUILib::ComboBox::setButtonColor(const SDL_Color& color) {
    buttonColor = color;
}

void GUILib::ComboBox::setFont(TTF_Font* font) 
{
    textFont = font;
    if (!mainButton) return;
    mainButton->changeFont(textFont);
    for (const auto& button : options) {
        if (!button) continue;
        button->changeFont(textFont);
    }
}

GUILib::ComboBox::ComboBox() : optionBoxColor(), buttonColor(), textFont()
{
    shouldRenderChildren = true;
}

std::string GUILib::ComboBox::getSelectedOption() const {
    return selectedOption;
}