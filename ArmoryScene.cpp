#include "ArmoryScene.h"

#include "images.h"


ArmoryScene::ArmoryScene() : Scene { Armory }, 
quit_button { Quit, L"Quit", { 15, 15 }, 80, 40 },
equip_button { Equip, L"Equip", { 0, 0 }, 0, 0 },
selected_weapon_button_index { -1 },
background_image { L"./res/Armory_background(temp).jpg" },
player_preview { L"./res/commando_right.png" },
preview_area { 0, 0, 0, 0 }, weapon_list_view_area { 0, 0, 0, 0 } {
    quit_button.border_color = Gray;
    quit_button.border_width = 2;
    quit_button.background_color = LightGray;
    quit_button.absolute = true;

    equip_button.border_color = Gray;
    equip_button.border_width = 2;
    equip_button.background_color = LightGray;
    equip_button.absolute = true;

    weapon_buttons.reserve(5);              // 장비가 추가될때마다 이 숫자 증가
    weapon_buttons.push_back(Button { Weapon0, L"SCAR-H", { }, 0, 0 });
    weapon_buttons.back().sprite = new Sprite { L"./res/scar_h_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    weapon_buttons.push_back(Button { Weapon1, L"M16", {}, 0, 0 });
    weapon_buttons.back().sprite = new Sprite{ L"./res/m16_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    weapon_buttons.push_back(Button { Weapon2, L"MP-44", {}, 0, 0 });
    weapon_buttons.back().sprite = new Sprite{ L"./res/mp44_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    weapon_buttons.push_back(Button { Weapon3, L"MG-42", {}, 0, 0 });
    weapon_buttons.back().sprite = new Sprite{ L"./res/mg42_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    weapon_buttons.push_back(Button { Weapon4, L"AWP", {}, 0, 0 });
    weapon_buttons.back().sprite = new Sprite{ L"./res/awp_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    for(auto& e : weapon_buttons) {
        e.background_color = LightGray;
        e.border_color = Gray;
        e.border_width = 2;
        e.absolute = true;
        e.padding = 5; 
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
    area = rectToSquare(area, Up);
    player_preview.draw(hdc, area);
    int w = percentOf(area.right-area.left, 40);
    area.left += w;
    area.right += w;

    Sprite* current_gun = nullptr;
    int idx;
    if(selected_weapon_button_index >= 0) {
        idx = selected_weapon_button_index+1;
    }
    else {
        idx = GUN_number;
    }

    switch(idx) {
    case scar_h:
        current_gun = new Sprite { L"./res/scar_h_right.png" };
        break;
    case m16:
        current_gun = new Sprite { L"./res/m16_right.png" };
        break;
    case mp_44:
        current_gun = new Sprite { L"./res/mp44_right.png" };
        break;
    case mg_42:
        current_gun = new Sprite { L"./res/mg42_right.png" };
        break;
    case awp:
        current_gun = new Sprite { L"./res/awp_right.png" };
        break;
    }

    if(current_gun != nullptr) {
        current_gun->fix_ratio = true;
        current_gun->draw(hdc, expandRatio(area, current_gun->source.GetWidth(), current_gun->source.GetHeight(), Left));
        delete current_gun;
    }

    TextBox weapon_info { L"무기 정보를 출력합니다.", { 0, 70 }, 100, 30 };

    weapon_info.show(hdc, preview_area);

    //// armory layout
    //Rectangle(hdc, weapon_list_view_area.left, weapon_list_view_area.top,
    //    weapon_list_view_area.right, weapon_list_view_area.bottom);

    for(auto& e : weapon_buttons) {
        e.show(hdc, weapon_list_view_area, Up);
    }

    RECT r = weapon_buttons[GUN_number-1].absoluteArea(valid_area);
    HBRUSH br = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    HBRUSH old_br = (HBRUSH)SelectObject(hdc, br);
    HPEN pen = CreatePen(PS_SOLID, 3, Red);
    HPEN old_pen = (HPEN)SelectObject(hdc, pen);
    Rectangle(hdc, r.left, r.top, r.right, r.bottom);
    SelectObject(hdc, old_pen);
    SelectObject(hdc, old_br);
    DeleteObject(pen);
    DeleteObject(br);
    if(selected_weapon_button_index >= 0 && selected_weapon_button_index != GUN_number-1) {
        r = weapon_buttons[selected_weapon_button_index].absoluteArea(valid_area);
        br = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
        old_br = (HBRUSH)SelectObject(hdc, br);
        pen = CreatePen(PS_SOLID, 3, Yellow);
        old_pen = (HPEN)SelectObject(hdc, pen);
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
        SelectObject(hdc, old_pen);
        SelectObject(hdc, old_br);
        DeleteObject(pen);
        DeleteObject(br);
    }
    
    equip_button.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);
}


void ArmoryScene::syncSize(const HWND& hWnd) {
    Scene::syncSize(hWnd);
    preview_area = { valid_area.left + 15, valid_area.top + 70,
        valid_area.left + (LONG)percentOf(horizontal_length, 50)-5, valid_area.bottom - 15 };
    weapon_list_view_area = { valid_area.left + (LONG)percentOf(horizontal_length, 50)+5, valid_area.top + 70,
        valid_area.right - 15, valid_area.bottom - 15 };

    int w = weapon_list_view_area.right - weapon_list_view_area.left;
    int h = weapon_list_view_area.bottom - weapon_list_view_area.top;
    RECT r = weapon_list_view_area;
    equip_button.width = percentOf(r.right - r.left, 60);
    equip_button.height = percentOf(r.bottom - r.top, 10);
    equip_button.position = { (double)r.left + percentOf(r.right - r.left, 20), (double)r.bottom - equip_button.height };

    w -= 5*(3+1);       // 한 줄에 3개, margin 5
    w /= 3;
    for(int i=0; i<weapon_buttons.size(); ++i) {
        weapon_buttons[i].position = { weapon_list_view_area.left + 5 + (5.0 + w)*(i%3), 
            weapon_list_view_area.top + 5 + (5.0 + percentOf(w, 70))*(i/3) };
        weapon_buttons[i].width = w;
        weapon_buttons[i].height = percentOf(w, 70);
        weapon_buttons[i].font_size = percentOf(weapon_buttons[i].height, 30);
    }
}

int ArmoryScene::clickL(const POINT& point) {
    RECT r = quit_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return quit_button.getID();
    }
    r = equip_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        if(selected_weapon_button_index >= 0) {
            GUN_number = selected_weapon_button_index+1;
        }
        return equip_button.getID();
    }

    for(int i=0; i<weapon_buttons.size(); ++i) {
        r = weapon_buttons[i].absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            if(selected_weapon_button_index == i) {
                selected_weapon_button_index = -1;
            }
            else {
                selected_weapon_button_index = i;
            }
            return weapon_buttons[i].getID();
        }
    }

    return None;
}