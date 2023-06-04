#pragma once

#include "Scene.h"
#include "Map.h"
#include "Player.h"

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
    bool paused;
    TextBox game_over_message;
    bool game_over;

    double play_time;
    clock_t start_time;
    clock_t end_time;

    Player player;

public:
    GameScene();

public:
    void setUp();
    void update(const POINT& point);
    void updatePlayer(const POINT& point);

    void moveLeft();
    void moveRight();

    void togglePauseState();
    void pause();
    void resume();

    bool isPaused() const;
    bool isGameOver() const;
    double getPlayTime() const;

private:
    //void updatePlayer(const POINT& mouse_cursor_point);
    //void updateEnemy();
    //void collisionCheck();
    //void playerCollisionCheck();
    //void enemyCollisionCheck();

protected:
    void draw(const HDC& hdc) const;
    RECT getViewArea() const;
    void drawScore(const HDC& hdc) const;
    void drawPauseScene(const HDC& hdc) const;
    void drawGameOverScene(const HDC& hdc) const;

public:
    int clickL(const POINT& point);
    int clickR(const POINT& point);
};