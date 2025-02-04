#include <lfmain.h>
#include <iostream>

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

bool isRunning = true;

GuiInstance manager;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	manager.initialize();

	mainWindow = SDL_CreateWindow("Program", 80, 80, 640, 480, 0);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);

	TTF_Font* mainFont = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 25);
	TTF_Font* mainFont9 = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 9);

	if (!mainFont || !mainFont9) {
		std::cout << "Font cannot be opened\n";
		return 1;
	}

	auto frame1Instance = manager.newObj("Frame", mainRenderer);
	auto editableBox1Instance = manager.newObj("EditableTextBox", mainRenderer);

	Frame* frame1 = dynamic_cast<Frame*>(frame1Instance.get());
	EditableTextBox* box1 = dynamic_cast<EditableTextBox*>(editableBox1Instance.get());
	
	frame1->size = { 0.25, 0.25, true };
	frame1->position = { 0.25, 0.25, true };
	frame1->frameColor = { 0, 0, 0, 255 };
	frame1->parent = std::nullopt;

	box1->size = { 0.5, 0.5, true };
	box1->position = { 0.25, 0.25, true };
	box1->parent = frame1;
	box1->boxColor = { 122, 122, 122, 255 };
	box1->textColor = { 0, 0, 0, 255 };
	box1->changeFont(mainFont9);

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
	box1->canBeDragged = false;

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
			frame1->handleEvent(e);
			box1->handleEvents(e);
		}
		frame1->canBeDragged = checkBox1.isChecked;
		box1->editable = checkBox2.isChecked;
		frame1->visible = true;
		SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mainRenderer);

		frame1->render();
		box1->render();
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
