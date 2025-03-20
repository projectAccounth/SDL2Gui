#include "guimanager.h"
#include "image.h"
#include "guiobject.h"
#include "textBox.h"
#include "button.h"

void GUILib::GuiInstance::initialize() {
    registerClass<Image>("Image");
    registerClass<Frame>("Frame");
    registerClass<EditableTextBox>("EditableTextBox");
    registerClass<TextBox>("TextBox");
    registerClass<ImageButton>("ImageButton");
    registerClass<TextButton>("TextButton");
    registerClass<CheckBox>("CheckBox");
}