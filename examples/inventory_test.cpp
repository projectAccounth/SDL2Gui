#include "guimanager.h"
#include <lfmain.h>

class Item {
protected:
	std::string name;
	int id;
public:
	Item() = default;
	explicit Item(std::string n, const int& id) : name(std::move(n)), id(id) {}

	void setName(const std::string& n) { name = n; }
	[[nodiscard]] std::string getName() const { return name; }

	void setId(const int& i) { id = i; }
	[[nodiscard]] int getId() const { return id; }
};

class Inventory {
protected:
	std::vector<Item> items;

	GUILib::EventEmitter events; // this thing has a lot of uses
public:
	Inventory() = default;

	template <typename... Args>
	void on(
		const std::string& eventName,
		std::function<void(Args...)> callback
	) {
		events.connect(eventName, std::move(callback));
	}

	template <typename... Args>
	void trigger(
		const std::string& eventName,
		Args&&... args
	) {
		events.fire(eventName, std::forward<Args>(args)...);
	}

	void addItem(const Item& item)
	{
		items.push_back(item);
		trigger("onChanged");
	}
	[[nodiscard]] std::vector<Item> getItems() const { return items; }

	void removeItems(int id)
	{
		const auto cachedItems = items;
		items.erase(std::remove_if(items.begin(), items.end(), [id](const Item& item)
		{
			return item.getId() == id;
		}), items.end());
		if (cachedItems.size() != items.size()) trigger("onChanged");
	}

	void removeItems(const std::string& name)
	{
		const auto cachedItems = items;
		items.erase(std::remove_if(items.begin(), items.end(), [name](const Item& item)
		{
			return item.getName() == name;
		}), items.end());
		if (cachedItems.size() != items.size()) trigger("onChanged");
	}

	void removeItems(const std::string& name, int id)
	{
		const auto cachedItems = items;
		items.erase(std::remove_if(items.begin(), items.end(), [name, id](const Item& item)
		{
			return item.getName() == name && item.getId() == id;
		}), items.end());
		if (cachedItems.size() != items.size()) trigger("onChanged");
	}
};

// Implement those yourself LMAO
enum class GridLayout : uint8_t
{
	GRID,
	FREE
};

class InventoryGui final : public GUILib::Frame {
private:
	std::shared_ptr<GUILib::ScrollingFrame> mainContentFrame;
	
	std::vector<std::shared_ptr<GUILib::TextButton>> invButtons;

	TTF_Font* buttonTextFont;
public:
	Inventory inventory;

	class Builder final : public GuiObject::Builder<Builder, InventoryGui> {};
	InventoryGui(): buttonTextFont()
	{
		shouldRenderChildren = true;
	}
	void init()
	{
		mainContentFrame = GUILib::ScrollingFrame::Builder()
			.setSize({ 1, 0.8, true })
			.setPosition({ 0, 0.2, true })
			.setParent(shared_from_this())
			.setRenderer(this->ref).build();

		const auto p = mainContentFrame;

		if (!p) std::terminate(); // crash if the GUI breaks

		p->setFrameColor({
			static_cast<Uint8>(frameColor.r + 15),
			static_cast<Uint8>(frameColor.g + 15),
			static_cast<Uint8>(frameColor.b + 15),
			255
			});
		p->setScrollbarColor({
			static_cast<Uint8>(frameColor.r - 15),
			static_cast<Uint8>(frameColor.g - 15),
			static_cast<Uint8>(frameColor.b - 15),
			255
			});
		p->setContentSize({ 1, 1.5, true });

		p->setVisible(true);
		p->setActive(true);

		std::cout << p->getEssentialInformation() << '\n';

		const auto closeButton = GUILib::TextButton::Builder()
			.setSize({ 0.05, 0.1, true })
			.setPosition({ 0.9, 0.05, true })
			.setParent(shared_from_this())
			.setVisible(true)
			.setActive(true)
			.setRenderer(this->ref).build();

		closeButton->setText("X");
		closeButton->changeButtonColor({ 155, 155, 155 ,255 });
		closeButton->changeHoverColor({ 133, 133, 133 , 255 });
		closeButton->changeTextColor({ 0, 0, 0, 255 });
		closeButton->changeFont(buttonTextFont);

		const auto label = GUILib::TextBox::Builder()
			.setSize({ 0.3, 0.2, true })
			.setPosition({ 0, 0, true })
			.setParent(shared_from_this())
			.setVisible(true)
			.setActive(true)
			.setRenderer(this->ref).build();

		label->changeFont(buttonTextFont);
		label->setBoxColor({ 0, 0, 0, 0 });
		label->setTextColor({ 0, 0, 0, 255 });
		label->setText("Inventory");

		closeButton->on("onClick", std::function([&](int, int)
		{
			setVisible(false);
		}));

		inventory.on("onChanged", std::function([this]()
		{
			updateInventoryRender();
		}));
	}

	void updateInventoryRender()
	{
		for (const auto& button : invButtons)
		{
			button->setParent(nullptr);
		}
		invButtons.clear();
		constexpr int totalCols = 9, totalRows = 4;
		int numItems = static_cast<int>(inventory.getItems().size());

		constexpr int totalCells = totalCols * totalRows;
		numItems = std::min(numItems, totalCells);

		// Determine cell size
		constexpr double cellWidth = 1.0 / totalCols;
		constexpr  double cellHeight = 1.0 / totalRows;


		for (int i = 0; i < numItems; ++i) {
			const int cellIndex = i;
			const int col = cellIndex % totalCols;
			const int row = cellIndex / totalCols;

			// Center item inside its cell
			const double posX = col * cellWidth + (cellWidth - 0.1) / 2.0;
			const double posY = row * cellHeight + (cellHeight - 0.2) / 2.0;

			const auto button = GUILib::TextButton::Builder()
				.setPosition({ posX, posY, true })
				.setSize({ 0.1, 0.2, true })
				.setRenderer(this->ref)
				.setParent(mainContentFrame)
				.setVisible(true)
				.setActive(true)
				.build();

			button->changeFont(buttonTextFont);
			button->changeButtonColor(this->frameColor);
			button->changeTextColor({ 0, 0, 0, 255 });
			button->setText(inventory.getItems()[i].getName());

			invButtons.push_back(button);
		}
	}

	void handleEvent(const SDL_Event& event) override
	{
		GuiObject::handleEvent(event);
	}

	void changeFont(TTF_Font* newFont) { buttonTextFont = newFont; }
};

int main(int argc, char* argv[])
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	Inventory exampleInventory;
	GUILib::SceneManager sceneManager(renderer);

	exampleInventory.addItem(Item("The", 0));
	exampleInventory.addItem(Item("Monaco", 1));
	exampleInventory.addItem(Item("based", 2));
	exampleInventory.addItem(Item("Youtuber", 3));

	const std::vector<std::string> str = { "who", "beat", "7-time", "world", "champion", "in", "equal", "machinery" };

	for (int i = 0; i < str.size(); ++i)
	{
		exampleInventory.addItem(Item(str[i], i));
	}

	int status = SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

	if (status != 0)
	{
		std::cout << SDL_GetError() << '\n';
		return status;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	TTF_Font* font = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	if (!font)
	{
		std::cout << TTF_GetError() << '\n';
		return 1;
	}

	sceneManager.updateRenderer(renderer);
	
	const auto invGui = InventoryGui::Builder()
		.setPosition({ 0.1, 0.1, true })
		.setSize({ 0.8, 0.8, true })
		.setRenderer(renderer)
		.setActive(true)
		.setVisible(true)
		.build();

	invGui->setFrameColor({ 177, 177, 177, 255 });
	invGui->changeFont(font);
	invGui->updateRenderer(renderer);
	invGui->init();

	invGui->inventory = exampleInventory;
	invGui->updateInventoryRender();

	std::cout << invGui->getEssentialInformation() << '\n';

	const auto openInvButton = GUILib::TextButton::Builder()
		.setSize({ 200, 50, false })
		.setPosition({ 0.2, 0, true })
		.setRenderer(renderer)
		.setVisible(true)
		.setActive(true)
		.build();

	openInvButton->changeButtonColor({ 200, 200, 200, 255 });
	openInvButton->changeHoverColor({ 122, 122, 122, 255 });
	openInvButton->changeTextColor({ 0, 0, 0, 255 });
	openInvButton->changeFont(font);
	openInvButton->setText("Whar");
	openInvButton->initialize(renderer);

	openInvButton->on("onClick", std::function([&](int, int)
	{
		std::cout << "Button clicked!\n";
		invGui->setVisible(!invGui->isVisible());
	}));

	const auto toggleDragInv = std::make_shared<GUILib::TextButton>(*openInvButton);

	toggleDragInv->setParent(openInvButton);
	toggleDragInv->resize({ 0.5, 1, true });
	toggleDragInv->move({ 1, 0, true });

	toggleDragInv->on("onClick", std::function([&](int, int)
	{
		invGui->setDraggable(!invGui->isDraggable());
	}));

	sceneManager.addBulk(openInvButton, invGui);

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
			default: break;
			}
			sceneManager.handleEvent(e);
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		sceneManager.render();

		SDL_RenderPresent(renderer);

		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	SDL_Quit();

	return 0;
}