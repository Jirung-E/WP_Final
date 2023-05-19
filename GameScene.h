#pragma once

#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include <list>
#include <ctime>


class GameScene : public Scene {
public:

private:
    Map map;
    Button resume_button;
    Button quit_button;
    bool paused;
    TextBox game_over_message;
    bool game_over;

    double kill_count;
    double play_time;
    clock_t start_time;
    clock_t end_time;

    Player player;

public:
    GameScene();

public:
    void setUp();
    void update(const POINT& point);

    void togglePauseState();
    void pause();
    void resume();

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
    ButtonID clickL(const POINT& point);
    ButtonID clickR(const POINT& point);
};