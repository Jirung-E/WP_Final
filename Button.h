#pragma once

#include "TextBox.h"


class Button : public TextBox {
private:
    int id;

public:
    Button(int id, const tstring& text = L"", const Point& position = { 0, 0 }, double width = 1, double height = 1);

public:
    int getID() const;
};