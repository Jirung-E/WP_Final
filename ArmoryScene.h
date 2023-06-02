#pragma once

#include "Scene.h"

#include <vector>

#include "Sprite.h"


class ArmoryScene : public Scene {
public:
    enum ButtonID {
        None, Quit, 
        Weapon0,
        Weapon1,
        Weapon2,
        Weapon3,
        Weapon4,
    };

private:
    Button quit_button;
    std::vector<Button> weapon_buttons;
    Sprite background_image;
    Sprite player_preview;
    RECT preview_area;
    RECT weapon_list_view_area;

public:
    ArmoryScene();

protected:
    void draw(const HDC& hdc) const;

public:
    void syncSize(const HWND& hWnd);
    int clickL(const POINT& point) const;
};