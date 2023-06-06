#pragma once

#include "TextBox.h"
#include "Sprite.h"


class Button : public TextBox {
public:
    Sprite* sprite;
    int padding;
    bool highlighting;

private:
    int id;

public:
    Button(int id, const tstring& text = L"", const Point& position = { 0, 0 }, double width = 1, double height = 1);
    Button(int id, Sprite* sprite, const Point& position = { 0, 0 }, double width = 1, double height = 1);

public:
    void show(const HDC& hdc, const RECT& valid_area, const Direction& bias = None) const;
    int getID() const;

protected:
    void drawBase(const HDC& hdc, const RECT& valid_area) const;
};