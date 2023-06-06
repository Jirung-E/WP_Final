#include "Button.h"


Button::Button(int id, const tstring& text, const Point& position, double width, double height) : 
    TextBox { text, position, width, height }, id { id }, sprite { nullptr }, padding { 0 }, highlighting { false } {

}

Button::Button(int id, Sprite* sprite, const Point& position, double width, double height) : 
    Button { id, L"", position, width, height } {
    this->sprite = sprite;
}


void Button::show(const HDC& hdc, const RECT& valid_area, const Direction& bias) const {
    drawBase(hdc, valid_area);
    if(sprite != nullptr) {
        sprite->draw(hdc, percentOf(absoluteArea(valid_area), 100-padding));
    }
    drawText(hdc, valid_area, bias);
}

int Button::getID() const {
    return id;
}


void Button::drawBase(const HDC& hdc, const RECT& valid_area) const {
    HBRUSH bg_br;
    if(transparent_background) {
        bg_br = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
    else {
        bg_br = CreateSolidBrush(background_color);
    }
    int style = transparent_border ? PS_NULL : PS_SOLID;
    HPEN bg_pen = CreatePen(style, border_width + (highlighting ? 2:0), border_color);
    HBRUSH old_br = (HBRUSH)SelectObject(hdc, bg_br);
    HPEN old = (HPEN)SelectObject(hdc, bg_pen);

    RECT rect = absoluteArea(valid_area);
    if(highlighting) {
        Rectangle(hdc, rect.left-3, rect.top-3, rect.right+3, rect.bottom+3);
    }
    else {
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }

    SelectObject(hdc, old_br);
    SelectObject(hdc, old);
    DeleteObject(bg_pen);
    DeleteObject(bg_br);
}