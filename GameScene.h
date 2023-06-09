#pragma once

#include "Scene.h"

//라운드 업 사운드 재생 유무
extern BOOL round_up_sound;

class GameScene : public Scene {
public:
    enum ButtonID {
        None, Resume, Quit
    };

private:
    Button resume_button;
    Button quit_button;
    bool is_paused;
    TextBox game_over_message;
    bool game_over;

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

protected:
    void draw(const HDC& hdc) const;
    void drawScore(const HDC& hdc) const;
    void drawPauseScene(const HDC& hdc) const;
    void drawGameOverScene(const HDC& hdc) const;

public:
    int clickL(const POINT& point);
    int clickR(const POINT& point);
};