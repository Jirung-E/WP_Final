#pragma once

#include "Scene.h"

#include "Sprite.h"


class MainScene : public Scene {
public:
    enum ButtonID {
        None, Start, Quit, Armory
    };

private:
    Button start_button;
    Button quit_button;
    Button armory_button;
    Sprite background_image;

public:
    MainScene();

protected:
    void draw(const HDC& hdc) const;

public:
    void syncSize(const HWND& hWnd);
    int clickL(const POINT& point) const;
    //int clickR(const POINT& point) const;
};