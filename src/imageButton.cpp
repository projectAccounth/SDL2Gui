#include "button.h"
#include "types.h"

int GUILib::ImageButton::nextId = 0;

void GUILib::ImageButton::initialize(SDL_Renderer*& renderer) {
    if (!renderer)
        return;

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
    if (!isVisible() || (parent && !parent->isVisible())) {
        return;
    }

    SDL_Texture* finalTexture = hovered ? hoverTexture : buttonTexture;

    SDL_RenderCopy(ref, finalTexture, nullptr, &objRect);

    SDL_DestroyTexture(finalTexture);
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
    GuiObject* parent,
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
    Button(),
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