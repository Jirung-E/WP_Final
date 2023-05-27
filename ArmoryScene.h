#pragma once

#include "Scene.h"

#include "Sprite.h"


class ArmoryScene : public Scene {
public:
    enum ButtonID {
        None, Quit,
    };

private:
    Button quit_button;
    //Button weapons[6];
    Sprite background_image;

public:
    ArmoryScene();

protected:
    void draw(const HDC& hdc) const;

public:
    int clickL(const POINT& point) const;
};