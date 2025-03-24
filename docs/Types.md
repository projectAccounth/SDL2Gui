### This section contains information about types used in the library.

#### `namespace GUILib`
The main namespace of the library.

#### `enum class HorizontalTextAlign`
The horizontal text alignment for text components.
Has 3 values: `LEFT`, `RIGHT` and `CENTER`.

#### `enum class VerticalTextAlign`
The vertical text alignment for text components.
Has 3 values: `UP`, `BOTTOM` and `CENTER`.

#### `struct UIUnit`
Members:\
`double sizeX`: The size on the X axis.\
`double sizeY`: The size on the Y axis.\
`bool isUsingScale`: Indicates whether the object is using a scale relative to the window's dimensions.\
If it is false, the object's origin will be at a position offset to the origin (0,0). Otherwise, its origin will be at the position relative to the window's size.