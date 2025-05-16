#include "button.h"
#include "types.h"

int GUILib::ImageButton::nextId = 0;

void GUILib::ImageButton::initialize(SDL_Renderer* renderer) {
    if (!renderer)
        return;

    if (defaultImgPath.empty()) {
        std::cout << "The default image path is empty. Please set it before using. Button ID: " << getId() << '\n';
        return;
    }
    else if (hoverImgPath.empty()) {
        std::cout << "The hover image path is empty. Consider setting it to something, could be the default image path. Button ID: " << getId() << '\n';
        return;
    }

    ref = renderer;

    if (hoverTexture) SDL_DestroyTexture(hoverTexture);
    if (buttonTexture) SDL_DestroyTexture(buttonTexture);
    SDL_Surface* imgSurf = IMG_Load(defaultImgPath.c_str());
    SDL_Surface* hoverSurf = IMG_Load(hoverImgPath.c_str());

    if (imgSurf == nullptr || hoverSurf == nullptr) {
        std::cout << "A problem occurred when creating one or more surface for image button, ID: "
            << id << ". Error: "
            << SDL_GetError() << '\n';
        exit(1);
    }
    hoverTexture = SDL_CreateTextureFromSurface(renderer, hoverSurf);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, imgSurf);

    if (hoverTexture == nullptr || buttonTexture == nullptr) {
        std::cout << "A problem occurred when creating one or more texture for image button, ID: "
            << id << ". Error: "
            << SDL_GetError() << '\n';
        exit(1);
    }

    SDL_FreeSurface(hoverSurf);
    SDL_FreeSurface(imgSurf);
}

void GUILib::ImageButton::render() {
    if (!shouldRender()) return;

    SDL_Texture* finalTexture = hovered ? hoverTexture : buttonTexture;

    SDL_RenderCopy(ref, finalTexture, nullptr, &objRect);

    SDL_DestroyTexture(finalTexture);

    GuiObject::render();
}

void GUILib::ImageButton::updateDefaultImgPath(const char* path) {
    defaultImgPath = path;
    initialize(ref);
    trigger("onPathUpdate", path);
}

void GUILib::ImageButton::updateHoverImgPath(const char* path) {
    hoverImgPath = path;
    initialize(ref);
    trigger("onHoverPathUpdate", path);
}

int GUILib::ImageButton::getId() const {
    return id;
}

GUILib::ImageButton::ImageButton(
    std::shared_ptr<GuiObject> parent,
    SDL_Renderer*& renderer,
    UIUnit size,
    UIUnit position,
    std::string defaultImageFilePath,
    std::string hoverImageFilePath
)
    :
    Button( parent, renderer, size, position ),
    buttonTexture(nullptr),
    hoverTexture(nullptr),
    defaultImgPath(defaultImageFilePath),
    hoverImgPath(hoverImageFilePath),
    id(nextId++)
{
    initialize(renderer);
}

GUILib::ImageButton::ImageButton():
    buttonTexture(nullptr),
    hoverTexture(nullptr),
    defaultImgPath(""),
    hoverImgPath(""),
    id(nextId++)
{
}

GUILib::ImageButton::~ImageButton() {
    if (buttonTexture)
        SDL_DestroyTexture(buttonTexture);
    if (hoverTexture)
        SDL_DestroyTexture(hoverTexture);
}