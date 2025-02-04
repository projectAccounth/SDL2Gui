#include "guimanager.h"
#include "lfmain.h"

void GuiInstance::registerClass(const std::string& className,
    std::function<std::unique_ptr<GuiObject>(SDL_Renderer*&)> creator) {
    factory[className] = std::move(creator);
}
std::unique_ptr<GuiObject> GuiInstance::newObj(const std::string& className, SDL_Renderer*& ref) {
    auto it = factory.find(className);
    if (it != factory.end()) {
        return it->second(ref);
    }
    else {
        std::cerr << "Error: Class " << className << " not registered.\n";
        return nullptr;
    }
}


void GuiInstance::initialize() {
    registerClass("TextButton", [](SDL_Renderer*& r) {
        return std::make_unique<TextButton>(
            UIUnit{},
            UIUnit{},
            std::nullopt,
            r,
            SDL_Color(),
            SDL_Color()
        );
    });
    registerClass("ImageButton", [](SDL_Renderer*& r) {
        return std::make_unique<ImageButton>(
            UIUnit{},
            UIUnit{},
            std::nullopt,
            r,
            "", ""
        );
    });
    registerClass("Image", [](SDL_Renderer*& r) {
        return std::make_unique<Image>(
            "",
            std::nullopt,
            r,
            UIUnit{},
            UIUnit{}
        );
    });
    registerClass("TextBox", [](SDL_Renderer*& r) {
        return std::make_unique<TextBox>(
            UIUnit{},
            UIUnit{},
            std::nullopt,
            r,
            SDL_Color()
        );
    });
    registerClass("EditableTextBox", [](SDL_Renderer*& r) {
        return std::make_unique<EditableTextBox>(
            UIUnit{},
            UIUnit{},
            std::nullopt,
            r,
            SDL_Color()
        );
    });
    registerClass("CheckBox", [](SDL_Renderer*& r) {
        return std::make_unique<CheckBox>(
            nullptr,
            SDL_Color(),
            SDL_Color(),
            std::nullopt,
            r
        );
    });
    registerClass("Frame", [](SDL_Renderer*& r) {
        return std::make_unique<Frame>(
            UIUnit{},
            UIUnit{},
            std::nullopt,
            r
        );
    });
}