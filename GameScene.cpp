#include "GameScene.h"

#include "Util.h"
#include "monster_info.h"

#include <sstream>


GameScene::GameScene() : Scene { Game },
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
        game_round = int(play_time/1000) / 10 + 1;
        if(mdx_r + mdx_big + mdx_air > 100) {
            game_over = true;
            ShowCursor(true);
        }
        //updatePlayer(point);
    }
    //updateEnemy();
    //collisionCheck();
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
    //drawBackground(hdc, White);

    RECT view_area = getViewArea();

    //map.draw(hdc, view_area);

    //for(auto e : enemies) {
    //    e->draw(hdc, map, view_area);
    //}
    //player.draw(hdc, view_area, map);

    drawScore(hdc);

    if(paused) {
        drawPauseScene(hdc);
    }
    else if(game_over) {
        drawGameOverScene(hdc);
    }
}

RECT GameScene::getViewArea() const {
    // 보이는 영역의 크기 설정
    // ...

    // 플레이어가 중앙에 오도록 평행이동
    // ...

    //return {
    //    LONG(floor(valid_area.left - px - A)),
    //    LONG(floor(valid_area.top - py - A)),
    //    LONG(floor(valid_area.right - px + A)),
    //    LONG(floor(valid_area.bottom - py + A))
    //};
    return valid_area;
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

    // 플레이 시간 출력
    //ss << L"ROUND" << int(play_time/1000) % 100 / 10 + 1 << L"  PlayTime: " << play_time/1000 << "\"";
    ss << L"ROUND" << int(play_time/1000) / 10 + 1;
    text = ss.str();
    score.text = ss.str();
    score.position.y += 5;
    score.show(hdc, valid_area);
    ss.str(L"");

    ss << L"PlayTime: " << play_time/1000 << "\"";
    text = ss.str();
    score.text = ss.str();
    score.position.y += 5;
    score.show(hdc, valid_area);
    ss.str(L"");

    // (테스트용) 몬스터 수 출력
    ss << L"Monsters: " << (mdx_r + mdx_big + mdx_air);
    text = ss.str();
    score.text = ss.str();
    score.position.y += 5;
    score.show(hdc, valid_area);
    ss.str(L"");
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

    tstring text;
    std::basic_stringstream<TCHAR> ss;

    // 플레이 시간 출력
    ss << L"PlayTime: " << play_time/1000 << "\"";
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