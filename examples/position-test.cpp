#include <lfmain.h>
#include <iostream>

using namespace GUILib;

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

bool isRunning = true;

GuiInstance manager;

std::vector<Image> imgs;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	manager.initialize();

	mainWindow = SDL_CreateWindow("Program", 80, 80, 640, 480, 0);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);

	TTF_Font* mainFont = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 25);
	TTF_Font* mainFont9 = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 9);

	if (!mainFont || !mainFont9) {
		std::cout << "Font cannot be opened\n";
		return 1;
	}

	auto frame1 = manager.create<Frame>("Frame", mainRenderer);
	auto box1 = manager.create<EditableTextBox>("EditableTextBox", mainRenderer);
	auto ap = manager.create<Image>("Image", mainRenderer);
	auto frame2 = manager.create<ScrollingFrame>("ScrollingFrame", mainRenderer);

	box1->on("onKeyInput", std::function<void(char)>([](char c) {
		std::cout << "Key " << c << " pressed!\n";
	}));

	ap->updatePath("./res/imgs/giri.webp");
	ap->setParent(nullptr);
	ap->move({ 0, 0, true });
	ap->resize({ 50, 50, false });

	frame2->move({ 0.5, 0.5, true });
	frame2->resize({ 100, 100, false });
	frame2->setContentSize({ 100, 200, false });
	frame2->setFrameColor({ 200, 200, 200, 255 });
	frame2->setScrollbarColor({ 233, 233, 233, 122 });
	
	Image ac = *ap;
	Image ae = ac;

	Image af = Image(
		nullptr,
		mainRenderer,
		{ 0, 0, true },
		{ 200, 200, false },
		"./res/imgs/giri.webp"
	);

	af.initialize(mainRenderer);
	af.setDraggable(true);
	af.toggleVisiblility(false);

	imgs.push_back(Image(
		nullptr,
		mainRenderer,
		{ 0, 0, true },
		{ 200, 200, false },
		"./res/imgs/giri.webp"
	));
	
	frame1->resize({ 0.25, 0.25, true });
	frame1->move({ 0.75, 0.75, true });
	frame1->setFrameColor({ 0, 0, 0, 255 });
	frame1->setParent(nullptr);

	box1->resize({ 0.5, 0.5, true });
	box1->move({ 0.25, 0.25, true });
	box1->setParent(frame1.get());
	box1->setBoxColor({ 122, 122, 122, 255 });
	box1->setTextColor({ 0, 0, 0, 255 });
	box1->changeFont(mainFont9);

	CheckBox checkBox1(
		nullptr,
		mainRenderer,
		UIUnit{ 25, 25, false },
		UIUnit({ .1, .1, true }),
		mainFont,
		SDL_Color{ 199, 199, 199, 255 },
		SDL_Color{ 0, 0, 0, 255 },
		'X'
	);
	CheckBox checkBox2(
		nullptr,
		mainRenderer,
		UIUnit{ 25, 25, false },
		UIUnit({ .2, .1, true }),
		mainFont,
		SDL_Color{ 199, 199, 199, 255 },
		SDL_Color{ 0, 0, 0, 255 },
		'O'
	);

	box1->setDraggable(false);
	ap->toggleVisiblility(true);

	for (auto& img : imgs) {
		img.initialize(mainRenderer);
	}

	frame2->addChild(&af);

	while (isRunning) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_WINDOWEVENT_CLOSE:
			case SDL_QUIT:
				isRunning = false;
				break;
			}
			checkBox1.handleEvent(e);
			checkBox2.handleEvent(e);
			frame1->handleEvent(e);
			box1->handleEvent(e);
			ap->handleEvent(e);
			frame2->handleEvent(e);
			// af.handleEvent(e);
		}
		frame1->setDraggable(checkBox1.isChecked());
		box1->setEditable(checkBox2.isChecked());
		ap->setDraggable(true);
		frame1->toggleVisiblility(true);
		SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mainRenderer);

		frame1->render();
		box1->render();
		checkBox1.render();
		checkBox2.render();
		ap->render();
		af.render();
		frame2->render();

		for (auto& img : imgs) {
			// img.render();
		}

		SDL_RenderPresent(mainRenderer);
	}

	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	TTF_CloseFont(mainFont);
	TTF_CloseFont(mainFont9);
	SDL_Quit();
	return 0;
}
