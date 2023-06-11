#pragma once

#include "Scene.h"

#include <vector>

#include "Sprite.h"


class ArmoryScene : public Scene {
public:
    enum ButtonID {
        None, Quit, Equip, Unlock,
        Weapon0,
        Weapon1,
        Weapon2,
        Weapon3,
        Weapon4,
        Weapon5,
        Weapon6
    };
    int selected_weapon_button_index;
    bool unlocked[7];

private:
    Button quit_button;
    Button equip_button;
    Button unlock_button;
    std::vector<Button> weapon_buttons;
    Sprite player_preview;
    RECT preview_area;
    RECT weapon_list_view_area;

public:
    ArmoryScene();

protected:
    void draw(const HDC& hdc) const;

public:
    void update(const POINT& point);
    void syncSize(const HWND& hWnd);
    int clickL(const POINT& point);
};