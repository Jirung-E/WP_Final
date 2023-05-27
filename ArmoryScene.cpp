#include "ArmoryScene.h"


ArmoryScene::ArmoryScene() : Scene { Armory }, 
quit_button { Quit, L"Quit", { 15, 15 }, 80, 40 },
background_image { L"./res/Armory_background(temp).jpg" } {
    quit_button.border_color = Gray;
    quit_button.border_width = 2;
    quit_button.background_color = LightGray;
    //quit_button.bias = Left;
    quit_button.absolute = true;
}


void ArmoryScene::draw(const HDC& hdc) const {
    background_image.draw(hdc, valid_area);

    // current equipment layout
    Rectangle(hdc, valid_area.left + 15, valid_area.top + 70,
        valid_area.left + percentOf(horizontal_length, 50)-5, valid_area.bottom - 15);

    // armory layout
    Rectangle(hdc, valid_area.left + percentOf(horizontal_length, 50)+5, valid_area.top + 70,
        valid_area.right - 15, valid_area.bottom - 15);
    
    quit_button.show(hdc, valid_area);
}


int ArmoryScene::clickL(const POINT& point) const {
    RECT r = quit_button.absoluteArea(valid_area);
    if(PtInRect(&r, point)) {
        return quit_button.getID();
    }
    return None;
}