#include <lfmain>

// Combo box test and tween service demonstration

namespace {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("Title", 55, 55, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font* font = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

    auto exampleBox = GUILib::ComboBox::Builder()
        .setRenderer(renderer)
        .setSize({ 120, 30, false })
        .setPosition({ 0.2, 0.2, true })
        .setActive(true)
        .setVisible(true)
        .build();

    exampleBox->setFont(font);

    exampleBox->addOption("Hello");
    exampleBox->addOption("World!");
    exampleBox->addOption("Hello");
    exampleBox->addOption("World!");

    exampleBox->setOptionBoxColor({ 100, 100, 100, 125 });
    exampleBox->setButtonColor({ 200, 200, 200, 255 });

    exampleBox->initialize(renderer);

    bool isRunning = true;

    Uint32 lastTicks = SDL_GetTicks();
    GUILib::TweenInfo comboTween;

    GUILib::TweenService::startTween(exampleBox, comboTween, GUILib::TweenType::POSITION, { 0.75, 0.75, true }, 5.0);

    while (isRunning) {
        SDL_Event e;

        Uint32 now = SDL_GetTicks();
        double deltaTime = (now - lastTicks) / 1000.0;
        lastTicks = now;

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_WINDOWEVENT_CLOSE:
            case SDL_QUIT:
                isRunning = false;
                break;
            }

            exampleBox->handleEvent(e);
        }

        GUILib::TweenService::updateTween(exampleBox, comboTween, deltaTime);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        exampleBox->render();

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}