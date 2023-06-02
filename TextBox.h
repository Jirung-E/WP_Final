#pragma once

#include "Util.h"
#include "Point.h"


class TextBox {
public:
    Point position;
    double width;
    double height;
    Direction bias;
    COLORREF background_color;
    COLORREF text_color;
    COLORREF border_color;
    int border_width;
    tstring text;
    bool transparent_background;
    bool transparent_border;
    UINT align;
    bool square;
    int bold;
    int italic;
    bool absolute;
    int font_size;

public:
    TextBox(const tstring& text = L"", const Point& position = { 0, 0 }, double width = 1, double height = 1);

public:
    void show(const HDC& hdc, const RECT& valid_area, const Direction& bias = None) const;
    RECT absoluteArea(const RECT& valid_area) const;

protected:
    void drawBase(const HDC& hdc, const RECT& valid_area) const;
    void drawText(const HDC& hdc, const RECT& valid_area, const Direction& bias = None) const;
};