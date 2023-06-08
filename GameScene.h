#pragma once
#include <windows.h>
#include "Scene.h"
#include "Map.h"

#include <list>
#include <ctime>


class GameScene : public Scene {
public:
    enum ButtonID {
        None, Resume, Quit
    };

private:
    Map map;
    Button resume_button;
    Button quit_button;
    bool is_paused;
    TextBox game_over_message;
    bool game_over;

    int score;          // ���带 �����ϴ� ����.
    int player_exp_first;          // ���� ���� ������ �÷��̾� ����ġ

    double play_time;
    clock_t start_time;
    clock_t end_time;

public:
    GameScene();

public:
    void setUp();
    void update(const POINT& point);

    void togglePauseState();
    void pause();
    void resume();

    bool isPaused() const;
    bool isGameOver() const;
    double getPlayTime() const;

protected:
    void draw(const HDC& hdc) const;
    void drawScore(const HDC& hdc) const;
    void drawPauseScene(const HDC& hdc) const;
    void drawGameOverScene(const HDC& hdc) const;

public:
    int clickL(const POINT& point);
    int clickR(const POINT& point);
};