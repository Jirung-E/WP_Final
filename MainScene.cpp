#include "MainScene.h"

#include "Util.h"


MainScene::MainScene() : Scene { Main }, 
start_button { Start, L"Start", { 25, 40 }, 50, 20 }, 
quit_button { Quit, L"Quit", { 35, 70 }, 30, 15 },
armory_button { Armory, L"Armory", { -30, 40 }, 50, 20 },
shop_button { Shop, L"Shop", { 80, 40 }, 50, 20 },
background_image { L"./res/Armory_background(temp).jpg" } {
    start_button.border_color = Gray;
    start_button.border_width = 10;
    start_button.background_color = LightGray;

    quit_button.border_color = Gray;
    quit_button.border_width = 10;
    quit_button.background_color = LightGray;

    armory_button.border_color = Gray;
    armory_button.border_width = 10;
    armory_button.background_color = LightGray;

    shop_button.border_color = Gray;
    shop_button.border_width = 10;
    shop_button.background_color = LightGray;
}


void MainScene::draw(const HDC& hdc) const {
    //drawBackground(hdc, White);
    background_image.draw(hdc, valid_area);

    start_button.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);
    armory_button.show(hdc, valid_area);
    shop_button.show(hdc, valid_area);
}


int MainScene::clickL(const POINT& point) const {
    RECT r = start_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return start_button.getID();
    }
    r = quit_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return quit_button.getID();
    }
    r = armory_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return armory_button.getID();
    }
    r = shop_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return shop_button.getID();
    }
    return None;
}

//int MainScene::clickR(const POINT& point) const {
//    return ButtonID();
//}