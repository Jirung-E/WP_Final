#include "Button.h"


Button::Button(int id, const tstring& text, const Point& position, double width, double height) : 
    TextBox { text, position, width, height }, id { id }, sprite { nullptr }, padding { 0 } {

}

Button::Button(int id, Sprite* sprite, const Point& position, double width, double height) : 
    Button { id, L"", position, width, height } {
    this->sprite = sprite;
}


void Button::show(const HDC& hdc, const RECT& valid_area) const {
    drawBase(hdc, valid_area);
    if(sprite != nullptr) {
        sprite->draw(hdc, percentOf(absoluteArea(valid_area), 100-padding));
    }
    drawText(hdc, valid_area);
}

int Button::getID() const {
    return id;
}