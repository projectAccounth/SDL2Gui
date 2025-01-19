#include <lfmain.h>

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

bool isRunning = true;

int main(int argc, char* argv[]) {
	mainWindow = SDL_CreateWindow("Program", 0, 0, 640, 480, 0);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);

	Frame frame1 = Frame(
		UIUnit{ 0.25, 0.25, true },
		UIUnit{ 0.25, 0.25, true },
		mainRenderer,
		mainRenderer,
		SDL_Color{ 0, 0, 0, 255 }
	);

	Frame frame2 = Frame(
		UIUnit{ 0.25, 0.25, true },
		UIUnit{ 0.25, 0.25, true },
		frame1,
		mainRenderer,
		SDL_Color{ 122, 122, 122, 255 }
	);

	while (isRunning) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_WINDOWEVENT_CLOSE:
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}

		frame1.render(mainRenderer);
		frame2.render(mainRenderer);
	}

	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}