### This section contains information about button classes in the library.

#### `class Button : public GuiObject`
The abstract button class. Practically useless.

##### `constructors`:
`Button()`: Default constructor.\
`Button(GuiObject* parent, SDL_Renderer*& renderer, UIUnit size, UIUnit position)`\
The constructor for the class, calls the constructor from the `GuiObject` superclass.

#### Methods and properties summary:

##### `protected std::function<void()> buttonAction`
Stores the button action for the deprecated set action API.\
Superseded by the `onClick` event, further instruction in `Event.md` file.

##### `protected std::function<void()> hoverAction`
Stores the hover action for the deprecated set action API.\
Superseded by the `onHover` event, further instruction in `Event.md` file.

##### `protected bool isClicked(int x, int y)`
A horribly-named helper function to check whether the mouse cursor is put on the button or not.

##### `protected void checkHover(int mouseX, int mouseY)`
Calls the `isClicked(int x, int y)` function to check for hovering. Assigns the `hovered` property.

##### `protected bool hovered`
The boolean that indicates whether the button is hovered. Cannot be assigned outside the class, and has a dedicated getter.

##### `public [deprecated] inline void setAction(const std::function<void()>& action)`
***Warning***: Deprecated API. Migrating to the Events API is recommended.\
Assigns the passed action to the `buttonAction` field.

##### `public [deprecated] inline void setHoverAction(const std::function<void()>& action)`
***Warning***: Deprecated API. Migrating to the Events API is recommended.\
Assigns the passed action to the `hoverAction` field.

##### `public void handleEvents(SDL_Event& e)`
Handles all event, from hovering, clicking, etc. Put in the `SDL_PollEvent` loop.

##### `public bool isHovered()`
Getter for `hovered`.

#### `class TextButton : public Button`
A basic text button.

##### `constructors`:
`TextButton()`: default constructor.\
```
TextButton(
	GuiObject* parent,
	SDL_Renderer*& renderer,
	UIUnit size,
	UIUnit position,
	SDL_Color buttonColor,
	SDL_Color hoverColor,
	SDL_Color textColor,
	std::string text,
	TTF_Font* textFont,
	HorizontalTextAlign alignX,
	VerticalTextAlign alignY
);
```
The main constructor.\
`parent`: The parent of the object. Not really a raw pointer, but rather a pointer to an existing object.\
`renderer`: A reference-to-pointer of the renderer.\
`alignX`: The horizontal alignment of the text in the box.\
`alignY`: The vertical alignment of the text in the box.\
`text`: Pre-initialized text.\
The rest is self-explantory.

#### Methods and properties summary:

##### `protected static int nextId`
Stores the next ID index for buttons, for the button ID system.

##### `protected int id`
The button's ID.

##### `protected HorizontalTextAlign xAlign`
Horizontal alignment of the text.

##### `protected VerticalTextAlign yAlign`
Vertical alignment of the text.

##### `protected std::string text`
The text of the box.

##### `protected SDL_Color buttonColor, hoverColor, textColor`
The color of the button normally, the button when hovered/or disabled, and the text color respectively.

##### `public void initialize(SDL_Renderer*& renderer)`
Pre-load the text. Must be called before rendering.

##### `public void render`
Renders the button.

##### `public void changeTextColor(const SDL_Color& color, SDL_Renderer*& renderer)`
Changes the text color on the associated renderer.

##### `public void changeButtonColor(const SDL_Color& color, SDL_Renderer*& renderer)`
Changes the button color on the associated renderer.

##### `public void changeHoverColor(const SDL_Color& color, SDL_Renderer*& renderer)`
Changes the button's hover color on the associated renderer.

##### `public inline SDL_Color getTextColor() const`

##### `public inline SDL_Color getHoverColor() const`

##### `public inline SDL_Color getButtonColor() const`

Self-explantory.

##### `public inline void setText(const std::string& str)`
Changes the text of the button.