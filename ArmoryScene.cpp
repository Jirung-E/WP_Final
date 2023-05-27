#include "ArmoryScene.h"


ArmoryScene::ArmoryScene() : Scene { Armory }, 
quit_button { Quit, L"Quit", { 15, 15 }, 80, 40 },
background_image { L"./res/Armory_background(temp).jpg" },
player_preview { L"./res/commando_right.png" },
preview_area { 0, 0, 0, 0 }, weapon_list_view_area { 0, 0, 0, 0 } {
    quit_button.border_color = Gray;
    quit_button.border_width = 2;
    quit_button.background_color = LightGray;
    quit_button.absolute = true;

    weapon_buttons.reserve(5);              // 장비가 추가될때마다 이 숫자 증가
    weapon_buttons.push_back(Button { Weapon0, L"SCAR_H", {}, 0, 0 });
    weapon_buttons.push_back(Button { Weapon1, L"M16", {}, 0, 0 });
    weapon_buttons.push_back(Button { Weapon2, L"MP_44", {}, 0, 0 });
    weapon_buttons.push_back(Button { Weapon3, L"MG_42", {}, 0, 0 });
    weapon_buttons.push_back(Button { Weapon4, L"P90", {}, 0, 0 });

    for(auto& e : weapon_buttons) {
        e.border_color = Gray;
        e.border_width = 2;
        e.absolute = true;
    }
}


void ArmoryScene::draw(const HDC& hdc) const {
    background_image.draw(hdc, valid_area);

    //// current equipment layout
    //Rectangle(hdc, preview_area.left, preview_area.top, 
    //    preview_area.right, preview_area.bottom);

    RECT area = preview_area;
    area.top = area.top + percentOf(preview_area.bottom - preview_area.top, 20);
    area.bottom = area.top + percentOf(preview_area.bottom - preview_area.top, 40);
    player_preview.draw(hdc, rectToSquare(area, Up));

    TextBox weapon_info { L"무기 정보를 출력합니다.", { 0, 70 },
        100, 30 };

    weapon_info.show(hdc, preview_area);

    //// armory layout
    //Rectangle(hdc, weapon_list_view_area.left, weapon_list_view_area.top,
    //    weapon_list_view_area.right, weapon_list_view_area.bottom);

    for(auto& e : weapon_buttons) {
        e.show(hdc, weapon_list_view_area);
    }
    
    quit_button.show(hdc, valid_area);
}


void ArmoryScene::syncSize(const HWND& hWnd) {
    Scene::syncSize(hWnd);
    preview_area = { valid_area.left + 15, valid_area.top + 70,
        valid_area.left + (LONG)percentOf(horizontal_length, 50)-5, valid_area.bottom - 15 };
    weapon_list_view_area = { valid_area.left + (LONG)percentOf(horizontal_length, 50)+5, valid_area.top + 70,
        valid_area.right - 15, valid_area.bottom - 15 };

    int w = weapon_list_view_area.right - weapon_list_view_area.left;
    w -= 5*(3+1);       // 한 줄에 3개, margin 5
    w /= 3;
    for(int i=0; i<weapon_buttons.size(); ++i) {
        weapon_buttons[i].position = { weapon_list_view_area.left + 5 + (5.0 + w)*(i%3), 
            weapon_list_view_area.top + 5 + (5.0 + percentOf(w, 70))*(i/3) };
        weapon_buttons[i].width = w;
        weapon_buttons[i].height = percentOf(w, 70);
    }
}

int ArmoryScene::clickL(const POINT& point) const {
    RECT r = quit_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return quit_button.getID();
    }
    return None;
}