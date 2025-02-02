#include <lfmain.h>
#include <iostream>

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

bool isRunning = true;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	mainWindow = SDL_CreateWindow("Program", 80, 80, 640, 480, 0);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);

	TTF_Font* mainFont = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 25);
	TTF_Font* mainFont9 = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 9);

	if (!mainFont) {
		std::cout << "Font cannot be opened\n";
		return 1;
	}

	Frame frame1(
		UIUnit{ 0.25, 0.25, true },
		UIUnit{ 0.25, 0.25, true },
		std::nullopt,
		mainRenderer,
		SDL_Color{ 0, 0, 0, 255 }
	);

	EditableTextBox box1(
		UIUnit({ 0.5, 0.5, true }),
		UIUnit({ 0.25, 0.25, true }),
		&frame1,
		mainRenderer,
		SDL_Color({ 122, 122, 122, 255 }),
		SDL_Color({ 0, 0, 0, 255 }),
		mainFont9
	);

	CheckBox checkBox1(
		mainFont,
		SDL_Color{ 199, 199, 199, 255 },
		SDL_Color{ 0, 0, 0, 255 },
		std::nullopt,
		mainRenderer,
		UIUnit({ .1, .1, true }),
		UIUnit{ 25, 25, false },
		'X'
	);
	CheckBox checkBox2(
		mainFont,
		SDL_Color{ 199, 199, 199, 255 },
		SDL_Color{ 0, 0, 0, 255 },
		std::nullopt,
		mainRenderer,
		UIUnit({ .2, .1, true }),
		UIUnit{ 25, 25, false },
		'O'
	);
	box1.canBeDragged = false;

	while (isRunning) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_WINDOWEVENT_CLOSE:
			case SDL_QUIT:
				isRunning = false;
				break;
			}
			checkBox1.handleEvents(e);
			checkBox2.handleEvents(e);
			frame1.handleEvent(e);
			box1.handleEvents(e);
		}
		frame1.canBeDragged = checkBox1.isChecked;
		box1.editable = checkBox2.isChecked;
		frame1.visible = true;
		SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mainRenderer);

		frame1.render();
		box1.render();
		checkBox1.render();
		checkBox2.render();

		SDL_RenderPresent(mainRenderer);
	}

	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	TTF_CloseFont(mainFont);
	TTF_CloseFont(mainFont9);
	SDL_Quit();
	return 0;
}