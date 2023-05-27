#pragma once

#include "GameScene.h"
#include "MainScene.h"


// �����ȯ, ��ư�� ���ȴ��� �� ����
class GameManager {
private:
    MainScene main_scene;
    GameScene game_scene;
    Scene* current_scene;
    POINT mouse_position;

    enum Timer {
        Display, UpdateGame
    };

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

    void setTimers(const HWND& hWnd);
    void timer(const HWND& hWnd, int id);

    void interrupt();

private:
    void gameStart(const HWND& hWnd);
    void lockCursor(const HWND& hWnd);
    void fixCursor(const HWND& hWnd);
    void releaseCursor();

    int buttonClicked(const POINT& point);

    void syncMousePosition(const HWND& hWnd);
};