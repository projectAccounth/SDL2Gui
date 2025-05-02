#include <lfmain.h>

int main(int argc, char* argv[])
{
	GUILib::GuiInstance Instance;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	int status = SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

	if (status != 0)
	{
		std::cout << SDL_GetError() << '\n';
		return status;
	}

	TTF_Font* font = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	if (!font)
	{
		std::cout << TTF_GetError() << '\n';
		return 1;
	}

	const std::shared_ptr box = Instance.create<GUILib::EditableTextBox>(renderer);


	box->on("onKeyInput", std::function([](char c)
	{
		std::cout << "Key " << c << " pressed\n";
	}));

	box->on("onSizeChange", std::function([&](const GUILib::UIUnit& t)
	{
		std::cout << (t == box->getSize()) << '\n';
	}));

	box->on("onSpecialKeyInput", std::function([]()
	{
		std::cout << "Special key pressed;\n";
	}));

	box->move({ 0, 0, true });
	box->resize({ .3, .5, true });
	box->setBoxColor({ 200, 200, 200, 255 });
	box->setTextColor({ 0, 0, 0, 255 });
	box->changeFont(font);
	box->setActive(true);
	box->setVisible(true);
	box->setParent(nullptr);
	box->setEditable(true);
	box->adjustTextAlignment(GUILib::HorizontalTextAlign::CENTER, GUILib::VerticalTextAlign::CENTER);

	
	const std::shared_ptr checkBox = Instance.create<GUILib::CheckBox>(renderer);

	checkBox->move({ 400, 400, false });
	checkBox->resize({ 50, 50, false });
	checkBox->changeButtonColor({ 177, 177, 177, 255 });
	checkBox->changeTextColor({ 0, 0, 0, 255 });
	checkBox->changeHoverColor({ 122, 122, 122, 255 });
	checkBox->setActive(true);
	checkBox->setVisible(true);
	checkBox->setParent(box);
	checkBox->changeFont(font);
	checkBox->initialize(renderer);

	const std::shared_ptr textButton = GUILib::TextButton::Builder()
		.setPosition({ 1, 0, true })
		.setSize({ 20, 20, false })
		.setParent(box)
		.setRenderer(renderer)
		.setVisible(true)
		.setActive(true)
		.build();

	textButton->changeButtonColor({ 200, 200, 200, 255 });
	textButton->changeTextColor({ 0, 0, 0, 255 });
	textButton->changeHoverColor({ 122, 122, 122, 255 });
	textButton->changeFont(font);
	textButton->initialize(renderer);
	textButton->setParent(box);

	textButton->on("onClick", std::function([box](int, int)
	{
		box->reset();
	}));

	const std::shared_ptr<GUILib::GuiObject> frame = GUILib::Frame::Builder()
		.setPosition({ .5, 0, true })
		.setSize({ 20, 150, false })
		.setParent(box)
		.setRenderer(renderer)
		.setVisible(true)
		.setActive(true)
		.build();

	std::static_pointer_cast<GUILib::Frame>(frame)->setFrameColor({ 0, 0, 0, 255 });

	bool isRunning = true;

	while (isRunning)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_WINDOWEVENT_CLOSE:
			case SDL_QUIT:
				isRunning = false;
				break;
			}
			box->handleEvent(e);

			box->setEditable(checkBox->isChecked());
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		box->render();
		frame->render();

		SDL_RenderPresent(renderer);

		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	SDL_Quit();

	return 0;
}