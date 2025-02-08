#include "button.h"
#include "types.h"

int ImageButton::nextId = 0;

void ImageButton::initialize(SDL_Renderer* renderer) {
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

void ImageButton::render() {
    if (!isVisible() || (parent && !parent.value()->visible)) {
        return;
    }

    SDL_Texture* finalTexture = hovered ? hoverTexture : buttonTexture;

    SDL_RenderCopy(ref, finalTexture, nullptr, &objRect);

    SDL_DestroyTexture(finalTexture);
}

void ImageButton::setAction(const std::function<void()>& actionFunction) {
    buttonAction = actionFunction;
}

void ImageButton::setHoverAction(const std::function<void()>& actionFunction) {
    hoverAction = actionFunction;
}

bool ImageButton::isClicked(int x, int y) {
    return (
        x > objRect.x &&
        x < (objRect.x + objRect.w) &&
        y > objRect.y &&
        y < (objRect.y + objRect.h)
    );
}
// too lazy to remove these foos
void ImageButton::checkHover(int mouseX, int mouseY) {
    hovered = isClicked(mouseX, mouseY);
}

void ImageButton::handleEvents(SDL_Event& e) {
    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    handleEvent(e);

    if (!((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) && active && visible)) {
        return;
    }
    if (x > objRect.x && x < (objRect.x + objRect.w) &&
        y > objRect.y && y < (objRect.y + objRect.h)) {
        hovered = true;
        if (hoverAction) hoverAction();
    }
    else {
        hovered = false;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && hovered &&
        e.button.button == SDL_BUTTON_LEFT) {
        if (buttonAction) buttonAction();
    }
}

void ImageButton::updateDefaultImgPath(const char* path, SDL_Renderer*& renderer) {
    defaultImgPath = path;
    initialize(renderer);
}

void ImageButton::updateHoverImgPath(const char* path, SDL_Renderer*& renderer) {
    hoverImgPath = path;
    initialize(renderer);
}

int ImageButton::getId() const {
    return id;
}

ImageButton::ImageButton(
    UIUnit size,
    UIUnit position,
    std::optional<GuiObject*> parent,
    SDL_Renderer*& renderer,
    std::string defaultImageFilePath,
    std::string hoverImageFilePath
)
    :
    GuiObject( size, position, parent, renderer ),
    hovered(false),
    buttonTexture(nullptr),
    hoverTexture(nullptr),
    defaultImgPath(defaultImageFilePath),
    hoverImgPath(hoverImageFilePath),
    id(nextId++)
{
    initialize(renderer);
}

ImageButton::ImageButton():
    GuiObject(),
    hovered(false),
    buttonTexture(nullptr),
    hoverTexture(nullptr),
    defaultImgPath(""),
    hoverImgPath(""),
    id(nextId++)
{}

ImageButton::~ImageButton() {
    SDL_DestroyTexture(buttonTexture);
    SDL_DestroyTexture(hoverTexture);
}

bool ImageButton::isHovered() const {
    return hovered;
}