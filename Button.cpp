#include "Button.h"


Button::Button(int id, const tstring& text, const Point& position, double width, double height) : TextBox { text, position, width, height } {

}


int Button::getID() const {
    return id;
}