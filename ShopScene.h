#pragma once

#include "Scene.h"


class ShopScene : public Scene {
public:
    enum ButtonID {
        None, Quit
    };

private:
    Button resume_button;
    Button quit_button;

public:
};