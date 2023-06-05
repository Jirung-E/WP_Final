#include "GameScene.h"

#include "Util.h"
#include "monster_info.h"
#include "player_info.h"
#include "images.h"
#include "ammo.h"
#include "exp.h"

#include <sstream>

GameScene::GameScene() : Scene { Game }, score { 0 }, player_exp_first { 0 },
map { 16, 9 },      // 좌표계.  전체화면비율인 16:9에 맞춥니다.
resume_button { Resume, L"Resume", { 20, 30 }, 60, 15 }, quit_button { Quit, L"Quit", { 20, 60 }, 60, 15 },
game_over_message { L"Game Over", { 10, 30 }, 80, 15 },
player { { 8, 4.5 } } {
    setUp();
    resume_button.border_color = Gray;
    resume_button.border_width = 3;

    quit_button.border_color = Gray;
    quit_button.border_width = 3;

    game_over_message.text_color = Red;
    game_over_message.background_color = LightGray;
    game_over_message.bold = 4;

    player.addSprite(new Sprite { L"./res/commando_right.png" });
}


void GameScene::setUp() {
    paused = false;
    game_over = false;
    play_time = 0.0;
    start_time = 0;
    end_time = 0;
    health = 100;

    //흔들림 초기화
    shake_count = 0;
    shake_acc = 0;
    shake_effect = 0;

    // 플레이어 위치 및 상태 초기화
    CM_x = 700, CM_y = 600;
    BG_scanner = 1500;  
    CM_jump = 0;
    CM_move_dir = -1;
    CM_jump_acc = 28; 
    CM_jump = 0; 
    space_pressed = 0;
    is_land = 1;

    // 총 상태 초기화
    is_zoom = FALSE;
    avail_awp = FALSE;
    shoot_delay = 0;
    after_delay = 0;

    // 라운드 초기화
    game_round = 1;

    // 스코어 초기화
    score = 0;
    player_exp_first = experience;

    // 스폰된 몬스터 초기화
    mdx_r = 0;
    mdx_big = 0;
    mdx_air = 0;

    // 장탄수 초기화
    ammo = 0;
    reload = 0;
    r_pressed = 0;
    empty = 0;
    can_shoot = TRUE;

    //게임오버 씬 초기화
    death_x = -500;
    death_acc = 54;
}


void GameScene::update(const POINT& point) {
    if(game_over) {
        return;
    }
    if(paused) {
        return;
    }
    if(!game_over) {
        end_time = clock();
        play_time += end_time - start_time;
        start_time = clock();
        //game_round = int(play_time/1000) / 10 + 1;
        score += experience - player_exp_first;
        player_exp_first = experience;
        if(score >= game_round * 10) {
            score -= game_round * 10;
            game_round++;
        }
        if(health <= 0) {
            game_over = true;
            ShowCursor(true);
        }
    }
}

void GameScene::updatePlayer(const POINT& point) {
    player.move(map);
}


void GameScene::moveLeft() {
    player.move({ -0.1, 0 }, map);
}

void GameScene::moveRight() {
    player.move({ 0.1, 0 }, map);
}


void GameScene::togglePauseState() {
    if(game_over) {
        return;
    }
    if(paused) {
        resume();
    }
    else {
        pause();
    }
}

void GameScene::pause() {
    if(!game_over) {
        paused = true;
    }
    ShowCursor(true);                //커서 대신 조준점 보이기
}

void GameScene::resume() {
    if(!game_over) {
        paused = false;
        end_time = clock();
        start_time = clock();
    }
    ShowCursor(false);                //커서 대신 조준점 보이기
}

bool GameScene::isPaused() const {
    return paused;
}

bool GameScene::isGameOver() const {
    return game_over;
}

double GameScene::getPlayTime() const {
    return play_time/1000;
}


void GameScene::draw(const HDC& hdc) const {
    drawScore(hdc);

    if(paused) {
        drawPauseScene(hdc);
    }
    else if(game_over) {
        drawGameOverScene(hdc);
    }
}

void GameScene::drawScore(const HDC& hdc) const {
    TextBox score { L"", { 0, 0 }, 50, 5 };
    score.transparent_background = true;
    score.transparent_border = true;
    score.align = DT_LEFT;
    score.square = false;
    score.bold = 4;
    score.italic = true;

    tstring text;
    std::basic_stringstream<TCHAR> ss;

    // 킬 수 출력
    // ...

    // 라운드 출력
    ss << L"ROUND" << game_round << " [" << this->score << "/" << game_round * 10 << "]";
    text = ss.str();
    score.text = ss.str();
    score.position.y += 5;
    score.show(hdc, valid_area);
    ss.str(L"");

    //// (테스트용) 몬스터 수 출력
    //ss << L"Monsters: " << (mdx_r + mdx_big + mdx_air);
    //text = ss.str();
    //score.text = ss.str();
    //score.position.y += 5;
    //score.show(hdc, valid_area);
    //ss.str(L"");
}

void GameScene::drawPauseScene(const HDC& hdc) const {
    resume_button.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);
}

void GameScene::drawGameOverScene(const HDC& hdc) const {
    game_over_message.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);

    TextBox score { L"", { 0, 45 }, 100, 7 };
    score.transparent_background = true;
    score.transparent_border = true;
    score.bold = 4;
    score.text_color = White;

    tstring text;
    std::basic_stringstream<TCHAR> ss;

    // 라운드 출력
    ss << L"ROUND" << game_round << " [" << this->score << "/" << game_round * 10 << "]";
    text = ss.str();
    score.text = ss.str();
    score.show(hdc, valid_area);
    ss.str(L"");
}


int GameScene::clickL(const POINT& point) {
    if(paused) {
        RECT r = resume_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            return resume_button.getID();
        }
        r = quit_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            return quit_button.getID();
        }
        return None;
    }
    if(game_over) {
        RECT r = quit_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            return quit_button.getID();
        }
        return None;
    }

    return None;
}

int GameScene::clickR(const POINT& point) {
    if(paused) {
        return None;
    }
    if(game_over) {
        return None;
    }

    return None;
}