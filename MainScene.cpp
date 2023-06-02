#include "MainScene.h"

#include "Util.h"


MainScene::MainScene() : Scene { Main }, 
start_button { Start, L"Start", { 25, 25 }, 50, 50 }, 
quit_button { Quit, L"Quit", { 80, 30 }, 40, 40 },
armory_button { Armory, L"Armory", { -20, 30 }, 40, 40 },
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
}


void MainScene::draw(const HDC& hdc) const {
    background_image.draw(hdc, valid_area);

    start_button.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);
    armory_button.show(hdc, valid_area);
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
    return None;
}


void MainScene::syncSize(const HWND& hWnd) {
    Scene::syncSize(hWnd);

    start_button.font_size = percentOf(vertical_length, 20);
    quit_button.font_size = percentOf(vertical_length, 10);
    armory_button.font_size = percentOf(vertical_length, 10);
}