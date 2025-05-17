#pragma once

#include "GuiObject.h"
#include "Frame.h"
#include "Button.h"

namespace GUILib {

    class ComboBox : public Frame {
    private:
        std::vector<std::shared_ptr<TextButton>> options;
        std::shared_ptr<TextButton> mainButton;
        std::shared_ptr<ScrollingFrame> optionBox;

        std::string selectedOption;

        SDL_Color optionBoxColor;
        SDL_Color buttonColor;

        TTF_Font* textFont;

        bool isOptionBoxShown = false;
        int optionBoxLength = 3;
    public:
        ComboBox();

        class Builder final : public GUILib::GuiObject::Builder<Builder, ComboBox> {};

        void initialize(SDL_Renderer* renderer) override;

        void addOption(const std::string& optionName);

        void removeOption(size_t index);

        void removeOptionByName(const std::string& text);

        void handleEvent(const SDL_Event& e) override;

        [[nodiscard]] int getOptionBoxLength() const;
        void setOptionBoxLength(const int& val);

        [[nodiscard]] bool getOptionBoxVisibility() const;
        void setOptionBoxVisibility(const bool& val);

        [[nodiscard]] SDL_Color getOptionBoxColor() const;
        void setOptionBoxColor(const SDL_Color& color);

        [[nodiscard]] SDL_Color getButtonColor() const;
        void setButtonColor(const SDL_Color& color);

        [[nodiscard]] std::string getSelectedOption() const;

        void setFont(TTF_Font* font);

        ComboBox(const ComboBox&) = default;
        ComboBox& operator=(const ComboBox&) = default;

        ComboBox(ComboBox&&) noexcept = default;
        ComboBox& operator=(ComboBox&&) noexcept = default;
    };
}