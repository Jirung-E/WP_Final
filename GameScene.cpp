#include "GameScene.h"

#include "Util.h"
#include "monster_info.h"
#include "player_info.h"
#include "images.h"
#include "ammo.h"
#include "exp.h"

#include <sstream>

GameScene::GameScene() : Scene { Game }, score { 0 }, player_exp_first { 0 },
map { 16, 9 },      // ��ǥ��.  ��üȭ������� 16:9�� ����ϴ�.
resume_button { Resume, L"��", { 20, 30 }, 60, 15 }, quit_button { Quit, L"��]", { 20, 70 }, 60, 15 },
game_over_message { L"DEAD", { 0, 25 }, 100, 20 } {
    setUp();
    resume_button.border_color = Gray;
    resume_button.border_width = 3;
    resume_button.bold = 2;

    quit_button.border_color = Gray;
    quit_button.border_width = 3;
    quit_button.bold = 2;

    game_over_message.text_color = Red;
    game_over_message.transparent_background = true;
    game_over_message.transparent_border = true;
    game_over_message.bold = 2;
}


void GameScene::setUp() {
    is_paused = false;
    game_over = false;
    play_time = 0.0;
    start_time = 0;
    end_time = 0;
    health = 100;

    //��鸲 �ʱ�ȭ
    shake_count = 0;
    shake_acc = 0;
    shake_effect = 0;
    ss_x = 0;
    ss_y = 0;

    // �÷��̾� ��ġ �� ���� �ʱ�ȭ
    CM_x = 700; CM_y = 600;
    BG_scanner = 1500;  
    CM_jump = 0;
    CM_move_dir = -1;
    CM_jump_acc = 28; 
    CM_jump = 0; 
    space_pressed = 0;
    is_land = 1;
    kill_count = 0;

    // �� ���� �ʱ�ȭ
    is_zoom = FALSE;
    avail_awp = FALSE;
    shoot_delay = 0;
    after_delay = 0;
    var = 0;
    cat_ready = 0;
    awp_cat_delay = 0;

    // ���� �ʱ�ȭ
    game_round = 1;

    // ���ھ� �ʱ�ȭ
    score = 0;
    player_exp_first = experience;

    // ������ ���� �ʱ�ȭ
    mdx_r = 0;
    mdx_big = 0;
    mdx_air = 0;
    ddx = 0;
    cdx = 0;
    delete_delay = 0;
    cat_delete_delay = 0;

    // ��ź�� �ʱ�ȭ
    ammo = 0;
    reload = 0;
    r_pressed = 0;
    empty = 0;
    reload_x = 0;
    can_shoot = TRUE;

    //���ӿ��� �� �ʱ�ȭ
    death_x = -500;
    death_acc = 54;

    //����ź �ʱ�ȭ
    is_throw = FALSE;
    is_boom = FALSE;
    set_grenade = FALSE;
    able_grenade = TRUE;
    gren_time = 0;
    gren_delay = 0;

    //�� ���� ��Ʈ�� ȭ�� �ʱ�ȭ
    is_intro = FALSE;
}


void GameScene::update(const POINT& point) {
    if(game_over) {
        RECT r = quit_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            quit_button.highlighting = true;
        }
        else {
            quit_button.highlighting = false;
        }
        return;
    }
    if(is_paused) {
        RECT r = resume_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            resume_button.highlighting = true;
        }
        else {
            resume_button.highlighting = false;
        }
        r = quit_button.absoluteArea(valid_area);
        if(PtInRect(&r, point)) {
            quit_button.highlighting = true;
        }
        else {
            quit_button.highlighting = false;
        }
        return;
    }
    if(!game_over) {
        //end_time = clock();
        //play_time += end_time - start_time;
        //start_time = clock();

        //EXP ��� ų ���� ���� ����
        //���� * 5���� ų ���� �������� ���� ����� �ö󰣴�.
        if(kill_count >= game_round * 5) {
            kill_count = 0;
            game_round++;
            //���� ���� �� WM_TIMER���� ���� �� �ִϸ��̼� ���
            round_up = TRUE;
            round_size = 120;
            round_x = 550;
        }
        if(health <= 0) {
            game_over = true;
            ShowCursor(true);
            health = 100; //��� ���� �ߺ� ��� ����
        }
    }
}


void GameScene::togglePauseState() {
    if(game_over) {
        return;
    }
    if(is_paused) {
        resume();
    }
    else {
        pause();
    }
}

void GameScene::pause() {
    if(!game_over) {
        is_paused = true;
    }
    ShowCursor(true);                //Ŀ�� ��� ������ ���̱�
}

void GameScene::resume() {
    if(!game_over) {
        is_paused = false;
        end_time = clock();
        start_time = clock();
    }
    ShowCursor(false);                //Ŀ�� ��� ������ ���̱�
}

bool GameScene::isPaused() const {
    return is_paused;
}

bool GameScene::isGameOver() const {
    return game_over;
}

double GameScene::getPlayTime() const {
    return play_time/1000;
}


void GameScene::draw(const HDC& hdc) const {
    drawScore(hdc);

    if(is_paused) {
        drawPauseScene(hdc);
    }
    else if(game_over) {
        drawGameOverScene(hdc);
    }
}

//�ΰ��� UI ���ϼ��� ���� ����
void GameScene::drawScore(const HDC& hdc) const {
    if (!game_over && !is_paused) {
        TCHAR lpout[100];
        HFONT hfont, oldfont;
        hfont = CreateFont(round_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���� ���"));
        oldfont = (HFONT)SelectObject(hdc, hfont);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));

        //���� ���
        wsprintf(lpout, L"ROUND %d", game_round);
        for (int i = -3; i <= 3; i++)
            for (int j = -4; j <= 4; j++)
                TextOut(hdc, ss_x + round_x + i, ss_y + landing_shake + 5 + j, lpout, lstrlen(lpout));
        if (round_up == FALSE)  SetTextColor(hdc, RGB(255, 255, 255));
        else if (round_up == TRUE) SetTextColor(hdc, RGB(0, 255, 0));
        TextOut(hdc, round_x + ss_x, 5 + ss_y + landing_shake, lpout, lstrlen(lpout));

        SelectObject(hdc, oldfont);
        DeleteObject(hfont);

        hfont = CreateFont(40, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���� ���"));
        oldfont = (HFONT)SelectObject(hdc, hfont);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));

        //������ �ʿ��� ų �� ���
        wsprintf(lpout, L"%d KILLS to GO", game_round * 5 - kill_count);
        for (int i = -3; i <= 3; i++)
            for (int j = -3; j <= 3; j++)
                TextOut(hdc, ss_x + 350 + i, ss_y + landing_shake + 10 + j, lpout, lstrlen(lpout));
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 350 + ss_x, 10 + ss_y + landing_shake, lpout, lstrlen(lpout));

        SelectObject(hdc, oldfont);
        DeleteObject(hfont);
    }
}

void GameScene::drawPauseScene(const HDC& hdc) const {
    resume_button.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);
}

//���� ���� �� ���� ���带 ǥ�� �ϴ� ������ ����
void GameScene::drawGameOverScene(const HDC& hdc) const {
    game_over_message.show(hdc, valid_area);
    quit_button.show(hdc, valid_area);

    //���� ���� ������ ��ġ
    double xpos = 0, ypos = 0;

    //���� �ڸ����� ���� ��ġ�� ���ݾ� �ٸ�
    //1�ڸ���
    if (game_round < 10) {
        xpos = 38; ypos = 45;
    }
    //2�ڸ���
    if (game_round >= 10) {
        xpos = 37, ypos = 45;
    }

    TextBox score { L"", { 0, 45 }, 100, 7 };
    //���� ���� ���� ����ϴ� ��ġ �� ũ��
    TextBox score2{ L"", { xpos, ypos }, 25, 25 };
    score.transparent_background = true;
    score.transparent_border = true;
    score.bold = 4;
    score.text_color = White;

    //���� ����
    score2.transparent_background = true;
    score2.transparent_border = true;
    score2.bold = 4;
    score2.text_color = White;

    tstring text, text2;
    std::basic_stringstream<TCHAR> ss;
    std::basic_stringstream<TCHAR> ss2;

    // 'TOTAL ROUNDS' ���
    ss << L"TOTAL ROUNDS";
    text = ss.str();
    score.text = ss.str();
    score.show(hdc, valid_area);
    ss.str(L"");
    
    //���� ���� ���
    ss2 << game_round;
    text2 = ss2.str();
    score2.text = ss2.str();
    score2.show(hdc, valid_area);
    ss2.str(L"");
}


int GameScene::clickL(const POINT& point) {
    if(is_paused) {
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
    if(is_paused) {
        return None;
    }
    if(game_over) {
        return None;
    }

    return None;
}