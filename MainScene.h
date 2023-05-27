#pragma once

#include "Scene.h"


class MainScene : public Scene {
public:
    enum ButtonID {
        None, Start, Quit, Armory, Shop
    };

private:
    Button start_button;
    Button quit_button;
    Button armory_button;
    Button shop_button;

public:
    MainScene();

protected:
    void draw(const HDC& hdc) const;

public:
    int clickL(const POINT& point) const;
    int clickR(const POINT& point) const;
};