### This section contains information about frame classes implemented in the library.

#### `class Frame`
An object which extends `GuiObject`.\
Serves the purpose of a fancy wrapper for a rectangle, with extra features like dragging.\

**Constructors summary:**

`Frame()`: Default constructor.\
```
Frame(
	GuiObject* parent,
	SDL_Renderer*& renderer,
	UIUnit size,
	UIUnit position,
	SDL_Color frameColor,
	bool isVisible,
	bool isActive
)
```
Main constructor. Self explanatory.\
**Methods and properties summary:**

##### `private SDL_Color frameColor`
Frame's color.

##### `public inline void setFrameColor(const SDL_Color& color)`
Sets the frame color.

##### `public inline SDL_Color getFrameColor() const`
Gets the frame color.

##### `public void render() override`
Renders the frame.

#### `class ScrollingFrame`
Still a concept. Stage 1 structure:

##### `private std::vector<GuiObject*> children`
The container for child elements. Might change any time.