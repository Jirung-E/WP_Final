#pragma once

#include "GameScene.h"
#include "MainScene.h"
#include "ArmoryScene.h"

//버튼 사운드 재생 유무 //TRUE일시 버튼 클릭음 재생
extern BOOL button_feed_clickScene;
extern BOOL button_feed_armory_button;
extern BOOL button_feed_armory_select;
extern BOOL button_feed_clickScene_start;
extern BOOL button_feed_clickScene_quit;
extern BOOL to_resume;
extern BOOL to_pause;

// 장면전환, 버튼이 눌렸는지 등 관리
class GameManager {
private:
    MainScene main_scene;
    GameScene game_scene;
    ArmoryScene armory_scene;
    Scene* current_scene;
    POINT mouse_position;

public:
    GameManager();
    ~GameManager();

public:
    void show(const HDC& hdc);
    void syncSize(const HWND& hWnd);

    void keyboardInput(const HWND& hWnd, int keycode);

    void clickScene(const HWND& hWnd, const POINT& point, const Direction& dir);
    void update(const HWND& hWnd);

    void quit(const HWND& hWnd);

    void interrupt();

    SceneID getCurrentSceneID() const;
    bool isPaused() const;
    bool isGameOver() const;

private:
    void gameStart(const HWND& hWnd);
    void lockCursor(const HWND& hWnd);
    void fixCursor(const HWND& hWnd);
    void releaseCursor();

    int buttonClicked(const POINT& point);

    void syncMousePosition(const HWND& hWnd);
};