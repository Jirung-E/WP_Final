#include "ArmoryScene.h"

#include "images.h"
#include "gun_info.h"
#include "exp.h"

BOOL button_feed_armory_button = FALSE;
BOOL button_feed_armory_select = FALSE;
BOOL cant_buy_sound = FALSE;
BOOL buy_sound = FALSE;

//폰트 두께 통일
ArmoryScene::ArmoryScene() : Scene { Armory }, 
quit_button{ Quit, L"[←", { 40, 15 }, 80, 80 },
equip_button { Equip, L"EQUIP", { 0, 0 }, 0, 0 },
unlock_button { Unlock, L"UNLOCK", { 0, 0 }, 0, 0 },
selected_weapon_button_index { -1 },
player_preview { L"./res/commando_right.png" },
preview_area { 0, 0, 0, 0 }, weapon_list_view_area { 0, 0, 0, 0 } {
    quit_button.border_color = Gray;
    quit_button.border_width = 2;
    quit_button.background_color = LightGray;
    quit_button.absolute = true;
    quit_button.bold = 4;

    equip_button.border_color = Gray;
    equip_button.border_width = 2;
    equip_button.background_color = LightGray;
    equip_button.absolute = true;
    equip_button.bold = 4;

    unlock_button.border_color = Gray;
    unlock_button.border_width = 2;
    unlock_button.background_color = LightGray;
    unlock_button.absolute = true;
    unlock_button.bold = 4;

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

    weapon_buttons.push_back(Button{ Weapon5, L"GARAND", {}, 0, 0 });
    weapon_buttons.back().sprite = new Sprite{ L"./res/m1_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    weapon_buttons.push_back(Button{ Weapon6, L"HBRUSH", {}, 0, 0 });
    weapon_buttons.back().sprite = new Sprite{ L"./res/hbrush_right.png" };
    weapon_buttons.back().sprite->fix_ratio = true;

    for(auto& e : weapon_buttons) {
        e.background_color = LightGray;
        e.border_color = Gray;
        e.border_width = 2;
        e.absolute = true;
        e.padding = 5; 
        e.bold = 4;
    }

    for(auto& e : unlocked) {
        e = false;
    }
    unlocked[scar_h-1] = true;
}

//폰트 두께 통일
void ArmoryScene::draw(const HDC& hdc) const {

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

    int pos_x_add = 0;
    int pos_y_add = 0;

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
        pos_x_add = -round(percentOf(area.right-area.left, 20));
        break;
    case awp:
        current_gun = new Sprite { L"./res/awp_right.png" };
        break;
    case m1:
        current_gun = new Sprite { L"./res/m1_right.png" };
        break;
    case h_brush:
        current_gun = new Sprite{ L"./res/hbrush_right.png" };
        break;
    }

    if(current_gun != nullptr) {
        RECT gun_draw_rect = expandRatio(area, current_gun->source.GetWidth(), current_gun->source.GetHeight(), Left);
        gun_draw_rect.left += pos_x_add;
        gun_draw_rect.right += pos_x_add;
        gun_draw_rect.top += pos_y_add;
        gun_draw_rect.bottom += pos_y_add;
        current_gun->fix_ratio = true;
        current_gun->draw(hdc, gun_draw_rect);
        delete current_gun;
    }

    RECT info_area = rectToSquare(preview_area);
    int font_size = percentOf(info_area.right - info_area.left, 5);

    TextBox current_exp_text { L"EXP", { 25, -8 }, 25, 12 };
    TextBox current_exp_value { std::to_wstring(experience), { 50, -8 }, 25, 12 };
    current_exp_text.font_size = font_size;
    current_exp_value.font_size = font_size;
    current_exp_text.bold = 4;
    current_exp_value.bold = 4;
    current_exp_text.background_color = LightGray;
    current_exp_value.background_color = brighter(LightGray, 20);
    current_exp_text.show(hdc, preview_area);
    current_exp_value.show(hdc, preview_area);
    

    TextBox damage_text { L"DAMAGE", { 0, 80 }, 25, 10 };
    TextBox damage_value { std::to_wstring(Gun::damage(idx)), { 25, 80 }, 25, 10 };
    damage_text.font_size = font_size;
    damage_value.font_size = font_size;
    damage_text.bold = 4;
    damage_value.bold = 4;
    damage_text.background_color = LightGray;
    damage_value.background_color = brighter(LightGray, 20);
    damage_text.show(hdc, preview_area);
    damage_value.show(hdc, preview_area);

    TextBox speed_text { L"RPM", { 50, 80 }, 25, 10 };  //기존의 속도 계산법 대신 실제 총기 데이터 사용
    TextBox speed_value { std::to_wstring(Gun::speed_info(idx)), { 75, 80 }, 25, 10};
    speed_text.font_size = font_size;
    speed_value.font_size = font_size;
    speed_text.bold = 4;
    speed_value.bold = 4;

    speed_text.background_color = LightGray;
    speed_value.background_color = brighter(LightGray, 20);
    speed_text.show(hdc, preview_area);
    speed_value.show(hdc, preview_area);

    TextBox ammo_text { L"BULLETS", { 0, 90 }, 25, 10 };
    TextBox ammo_value { std::to_wstring(Gun::max_ammo(idx)), { 25, 90 }, 25, 10 };
    ammo_text.font_size = font_size;
    ammo_value.font_size = font_size;
    ammo_text.bold = 4;
    ammo_value.bold = 4;

    ammo_text.background_color = LightGray;
    ammo_value.background_color = brighter(LightGray, 20);
    ammo_text.show(hdc, preview_area);
    ammo_value.show(hdc, preview_area);

    TextBox price_text { L"COST", { 50, 90 }, 25, 10 };
    TextBox price_value { std::to_wstring(Gun::price(idx)), { 75, 90 }, 25, 10 };
    price_text.font_size = font_size;
    price_value.font_size = font_size;
    price_text.bold = 4;
    price_value.bold = 4;

    if(unlocked[idx-1]) {
        price_text.text = L"-";
        price_value.text = L"-";
    }
    else {
        if(experience < Gun::price(idx)) {
            price_value.text_color = Red;
        }
    }
    price_text.background_color = LightGray;
    price_value.background_color = brighter(LightGray, 20);
    price_text.show(hdc, preview_area);
    price_value.show(hdc, preview_area);

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
    
    if(unlocked[idx-1]) {
        if(idx-1 != GUN_number-1) {
            equip_button.show(hdc, valid_area);
        }
    }
    else {
        unlock_button.show(hdc, valid_area);

    }

    Sprite lock { L"./res/lock_white.png" };
    Sprite blind { L"./res/blind.png" };
    for(int i=0; i<weapon_buttons.size(); ++i) {
        if(!unlocked[i]) {
            r = weapon_buttons[i].absoluteArea(valid_area);
            blind.source.Draw(hdc, r);
            r = rectToSquare(r);
            lock.source.Draw(hdc, r);
        }
    }

    quit_button.show(hdc, valid_area);
}


void ArmoryScene::update(const POINT& point) {
    RECT r = quit_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        quit_button.highlighting = true;
    }
    else {
        quit_button.highlighting = false;
    }

    r = equip_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        equip_button.highlighting = true;
    }
    else {
        equip_button.highlighting = false;
    }

    r = unlock_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        unlock_button.highlighting = true;
    }
    else {
        unlock_button.highlighting = false;
    }

    for(auto& e : weapon_buttons) {
        r = e.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            e.highlighting = true;
        }
        else {
            e.highlighting = false;
        }
    }
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

    unlock_button.width = percentOf(r.right - r.left, 60);
    unlock_button.height = percentOf(r.bottom - r.top, 10);
    unlock_button.position = { (double)r.left + percentOf(r.right - r.left, 20), (double)r.bottom - unlock_button.height };

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
        logo_acc = 30; //로고 애니메이션 값 초기화
        logo_y = -415;
        armory_to_main = TRUE;
        return quit_button.getID();
    }

    if(unlocked[selected_weapon_button_index]) {
        r = equip_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            if(selected_weapon_button_index >= 0) {
                GUN_number = selected_weapon_button_index+1;
                button_feed_armory_select = TRUE;
            }
            return equip_button.getID();
        }
    }
    else {
        r = unlock_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            if(selected_weapon_button_index >= 0) {
                if(experience >= Gun::price(selected_weapon_button_index+1)) {
                    experience -= Gun::price(selected_weapon_button_index+1);
                    GUN_number = selected_weapon_button_index+1;
                    unlocked[selected_weapon_button_index] = true;
                    buy_sound = TRUE;
                }
                else if (experience < Gun::price(selected_weapon_button_index + 1)) {
                    cant_buy_sound = TRUE;
                }
            }
            return unlock_button.getID();
        }
    }

    for(int i=0; i<weapon_buttons.size(); ++i) {
        r = weapon_buttons[i].absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            if(selected_weapon_button_index == i) {
                selected_weapon_button_index = -1;
                button_feed_armory_button = TRUE;
            }
            else {
                selected_weapon_button_index = i;
                button_feed_armory_button = TRUE;
            }
            return weapon_buttons[i].getID();
        }
    }

    return None;
}