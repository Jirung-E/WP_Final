#include "MainScene.h"
#include "images.h"
#include "Util.h"

//추후 메인에 로고를 넣기 위한 버튼 크기 조정
MainScene::MainScene() : Scene { Main }, 
start_button{ Start, L"▶", { 30, 50 }, 40, 40 },
quit_button{ Quit, L"→]", { 80, 55 }, 30, 30 },
armory_button{ Armory, L"ARMS", { -10, 55 }, 30, 30 }
{
    start_button.border_color = Gray;
    start_button.border_width = 10;
    start_button.background_color = LightGray;
    quit_button.bold = 2;

    quit_button.border_color = Gray;
    quit_button.border_width = 10;
    quit_button.background_color = LightGray;
    quit_button.bold = 2;

    armory_button.border_color = Gray;
    armory_button.border_width = 10;
    armory_button.background_color = LightGray;
    armory_button.bold = 2;
}


void MainScene::draw(const HDC& hdc) const {

    start_button.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);
    armory_button.show(hdc, valid_area);
}


void MainScene::update(const POINT& point) {
    RECT r = quit_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        quit_button.highlighting = true;
    }
    else {
        quit_button.highlighting = false;
    }
    r = start_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        start_button.highlighting = true;
    }
    else {
        start_button.highlighting = false;
    }
    r = armory_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        armory_button.highlighting = true;
    }
    else {
        armory_button.highlighting = false;
    }
}

void MainScene::syncSize(const HWND& hWnd) {
    Scene::syncSize(hWnd);

    start_button.font_size = percentOf(vertical_length, 20);
    quit_button.font_size = percentOf(vertical_length, 10);
    armory_button.font_size = percentOf(vertical_length, 10);
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
        logo_acc = 30; //로고 애니메이션 값 초기화
        logo_y = 50;
        main_to_armory = TRUE;
        return armory_button.getID();
    }
    return None; 
}