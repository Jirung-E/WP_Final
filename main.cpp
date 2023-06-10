#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include <math.h>
#include <random>
#include "GameManager.h"
#include "monster_info.h"//몬스터 정보 헤더
#include "ammo.h"        //총알 정보 헤더
#include "player_info.h" //플레이어 정보 헤더
#include "images.h"      //이미지 정보 헤더
#include "exp.h"         //경험치 정보 헤더, 경험치로 총기를 구입하고, 소비한다.
#include "gun_info.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment (lib, "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64/fmod_vc.lib")

FMOD::System* ssystem;
FMOD::Sound* scar_shoot, *m16_shoot, *mp44_shoot, *mg42_shoot, *awp_shoot, *dry_fire;
FMOD::Sound* scar_distance, * m16_distance, * mp44_distance, *mg42_distance, * awp_distance;
FMOD::Sound* rifle_reload, * lmg_reload, * sniper_reload, * sniper_bolt, * walk, * hit_sound, * jump, * exp_get, *land_sound, *zoom_sound, *unzoom_sound;
FMOD::Sound* hurt, *dead, *cat_hit_ground, *cat_stop, *ex_sound, *pin_sound;
FMOD::Sound* mst_idle_sound1, * mst_idle_sound2, *mst_attack_sound1, *mst_attack_sound2, *button_sound, *weapon_select, *weapon_button, *start_button, *quit_button;
FMOD::Sound* pause, * resume, *game_bgm, *main_bgm, *gameover_bgm, *pause_bgm, *next_round, *intro, *new_game, *woosh;

//gun sound
FMOD::Channel* ch_gun = 0;
//gun_distance sound
FMOD::Channel* ch_gun2 = 0;
//reload_sound
FMOD::Channel* ch_reload = 0;
//hit_sound
FMOD::Channel* ch_hit = 0;
//walk 
FMOD::Channel* ch_walk = 0;
//jump
FMOD::Channel* ch_jump = 0;
//land
FMOD::Channel* ch_land = 0;
//exp_get
FMOD::Channel* ch_exp = 0;
//next_round
FMOD::Channel* ch_round = 0;
//awp_zoom
FMOD::Channel* ch_zoom = 0;
//awp_no_zoom
FMOD::Channel* ch_dry = 0;
//hurt
FMOD::Channel* ch_hurt = 0;
//dead
FMOD::Channel* ch_dead = 0;
//monster_sound
FMOD::Channel* ch_mst_idle_sound = 0;
//monster_attack_sound
FMOD::Channel* ch_mst_attack_sound = 0;
//button_sound
FMOD::Channel* ch_button = 0;
//catridge sound
FMOD::Channel* ch_cat = 0;
//main bgm
FMOD::Channel* ch_bgm = 0;
//explode
FMOD::Channel* ch_explode = 0;

FMOD_RESULT result;
void* extradriverdata = 0;

//브금 중복 재생 방지
static BOOL main_bgm_on = FALSE;
static BOOL game_bgm_on = FALSE;
static BOOL gameover_bgm_on = FALSE;
static BOOL pause_bgm_on = FALSE;
//수류탄 폭발음 중복 재생 방지
static BOOL boom_sound = FALSE;
static BOOL intro_sound = FALSE;
static BOOL new_game_sound = FALSE;
static BOOL whoosh_sound = FALSE;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

static int walk_sound_delay; //발소리가 너무 빠르게 들리면 안되므로 딜레이를 줘 가며 재생
static int monster_sound_delay; //몬스터 idle 사운드 딜레이

std::random_device rd_main;
std::mt19937 gen(rd_main());

GameManager manager;
extern monster_info_regular mst_r[100]; //몬스터 정보를 저정하는 구조체, monster_info.h에 선언
extern monster_info_big mst_big[100];
extern monster_info_air mst_air[100];
extern dead_location dl[100];
extern gun_catridge gc[2000];

enum Timer {
	UPDATE
};

//마우스 좌표
static double mx, my;
//마우스 클릭 여부
static BOOL is_click = FALSE;
//키보드 키 검사
static bool left_pressed = false; static bool right_pressed = false;
static bool jumping = false; static bool shift_pressed = false;

//TRUE일 경우 일시정지 화면이 사라지는 애니메이션 재생
extern BOOL is_resumed;


//이미지 파일 로드
void IMG_FILE_LOAD() {
	BackGround.Load(L".\\res\\BackGround_wide.png");
	BG_paused.Load(L".\\res\\BackGround_paused.png");
	CM_paused.Load(L".\\res\\commando_paused.png");
	logo.Load(L".\\res\\logo.png");
	background_main.Load(L".\\res\\background_main.png");
	intro_logo.Load(L".\\res\\intro_logo.png");
	background_game_start.Load(L".\\res\\BackGround_game_start.png");
	CM_game_start.Load(L".\\res\\commando_game_start.png");
	logo_game_start.Load(L".\\res\\game_start_logo.png");

	commando_right.Load(L".\\res\\commando_right.png");
	commando_left.Load(L".\\res\\commando_left.png");
	commando_jump_right.Load(L".\\res\\commando_jump_right.png");
	commando_jump_left.Load(L".\\res\\commando_jump_left.png");
	commando_fire_right.Load(L".\\res\\commando_fire_right.png");
	commando_fire_left.Load(L".\\res\\commando_fire_left.png");
	commando_jump_fire_right.Load(L".\\res\\commando_jump_fire_right.png");
	commando_jump_fire_left.Load(L".\\res\\commando_jump_fire_left.png");
	commando_zoom_right.Load(L".\\res\\commando_zoom_right.png");
	commando_zoom_left.Load(L".\\res\\commando_zoom_left.png");

	monster_right.Load(L".\\res\\monster_right.png");
	monster_left.Load(L".\\res\\monster_left.png");
	monster_big_right.Load(L".\\res\\monster_big_right.png");
	monster_big_left.Load(L".\\res\\monster_big_left.png");
	monster_air_right[0].Load(L".\\res\\bat\\bat1_right.png");
	monster_air_right[1].Load(L".\\res\\bat\\bat2_right.png");
	monster_air_right[2].Load(L".\\res\\bat\\bat3_right.png");
	monster_air_left[0].Load(L".\\res\\bat\\bat1_left.png");
	monster_air_left[1].Load(L".\\res\\bat\\bat2_left.png");
	monster_air_left[2].Load(L".\\res\\bat\\bat3_left.png");


	SCAR_H_right.Load(L".\\res\\SCAR_H_right.png");
	SCAR_H_left.Load(L".\\res\\SCAR_H_left.png");
	M16_right.Load(L".\\res\\m16_right.png");
	M16_left.Load(L".\\res\\m16_left.png");
	MP44_right.Load(L".\\res\\mp44_right.png");
	MP44_left.Load(L".\\res\\mp44_left.png");
	MG42_right.Load(L".\\res\\mg42_right.png");
	MG42_left.Load(L".\\res\\mg42_left.png");
	AWP_right.Load(L".\\res\\awp_right.png");
	AWP_left.Load(L".\\res\\awp_left.png");

	indicator_back.Load(L".\\res\\indicator_back.png");
	ammo_icon.Load(L".\\res\\ammo_icon.png");
	exp_icon.Load(L".\\res\\exp.png");
	flame_right.Load(L".\\res\\flame_right.png");
	flame_left.Load(L".\\res\\flame_left.png");
	ammo_lmg_icon.Load(L".\\res\\ammo_lmg_icon.png");
	ammo_sniper_icon.Load(L".\\res\\ammo_sniper_icon.png");
	zoom_complited.Load(L".\\res\\zoom_complited.png");
	zoom_targeted.Load(L".\\res\\zoom_targeted.png");

	CM_dead.Load(L".\\res\\commando_dead.png");
	monster_dead_left.Load(L".\\res\\monster_dead_left.png");
	monster_big_dead_left.Load(L".\\res\\monster_big_dead_left.png");
	monster_dead_right.Load(L".\\res\\monster_dead_right.png");
	monster_big_dead_right.Load(L".\\res\\monster_big_dead_right.png");
	monster_air_dead.Load(L".\\res\\bat_dead.png");

	catridge[0].Load(L".\\res\\catridge\\catridge1.png");
	catridge[1].Load(L".\\res\\catridge\\catridge2.png");
	catridge[2].Load(L".\\res\\catridge\\catridge3.png");
	catridge[3].Load(L".\\res\\catridge\\catridge4.png");

	grenade[0].Load(L".\\res\\grenade\\grenade1.png");
	grenade[1].Load(L".\\res\\grenade\\grenade2.png");
	grenade[2].Load(L".\\res\\grenade\\grenade3.png");
	grenade[3].Load(L".\\res\\grenade\\grenade4.png");
	grenade[4].Load(L".\\res\\grenade\\grenade5.png");
	grenade[5].Load(L".\\res\\grenade\\grenade6.png");
	grenade[6].Load(L".\\res\\grenade\\grenade7.png");
	grenade[7].Load(L".\\res\\grenade\\grenade8.png");

	avail_grenade.Load(L".\\res\\grenade_able_icon.png");
	unavail_grenade.Load(L".\\res\\grenade_unable_icon.png");

	explode[0].Load(L".\\res\\explode\\ex1.png");
	explode[1].Load(L".\\res\\explode\\ex2.png");
	explode[2].Load(L".\\res\\explode\\ex3.png");
	explode[3].Load(L".\\res\\explode\\ex4.png");
	explode[4].Load(L".\\res\\explode\\ex5.png");
	explode[5].Load(L".\\res\\explode\\ex6.png");
	explode[6].Load(L".\\res\\explode\\ex7.png");
} 

//FMOD 세팅
void set_FMOD() {
	result = FMOD::System_Create(&ssystem); //--- 사운드 시스템 생성
	if (result != FMOD_OK) exit(0);
	ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata); //--- 사운드 시스템 초기화
	ssystem->createSound(".\\res\\sounds\\game_bgm.wav", FMOD_LOOP_NORMAL, 0, &game_bgm);
	ssystem->createSound(".\\res\\sounds\\main_bgm.wav", FMOD_LOOP_NORMAL, 0, &main_bgm);
	ssystem->createSound(".\\res\\sounds\\gameover_bgm.wav", FMOD_LOOP_NORMAL, 0, &gameover_bgm);
	ssystem->createSound(".\\res\\sounds\\pause_bgm.wav", FMOD_LOOP_NORMAL, 0, &pause_bgm);
	ssystem->createSound(".\\res\\sounds\\intro.wav", FMOD_DEFAULT, 0, &intro);
	ssystem->createSound(".\\res\\sounds\\into_the_game.wav", FMOD_DEFAULT, 0, &new_game);
	ssystem->createSound(".\\res\\sounds\\woosh.wav", FMOD_DEFAULT, 0, &woosh);


	ssystem->createSound(".\\res\\sounds\\scar_h.wav", FMOD_DEFAULT, 0, &scar_shoot);
	ssystem->createSound(".\\res\\sounds\\m16.wav", FMOD_DEFAULT, 0, &m16_shoot);
	ssystem->createSound(".\\res\\sounds\\mp44.wav", FMOD_DEFAULT, 0, &mp44_shoot);
	ssystem->createSound(".\\res\\sounds\\mg42.wav", FMOD_DEFAULT, 0, &mg42_shoot);
	ssystem->createSound(".\\res\\sounds\\awp.wav", FMOD_DEFAULT, 0, &awp_shoot);

	ssystem->createSound(".\\res\\sounds\\scar_h_distance.wav", FMOD_DEFAULT, 0, &scar_distance);
	ssystem->createSound(".\\res\\sounds\\m16_distance.wav", FMOD_DEFAULT, 0, &m16_distance);
	ssystem->createSound(".\\res\\sounds\\mp44_distance.wav", FMOD_DEFAULT, 0, &mp44_distance);
	ssystem->createSound(".\\res\\sounds\\mg42_distance.wav", FMOD_DEFAULT, 0, &mg42_distance);
	ssystem->createSound(".\\res\\sounds\\awp_distance.wav", FMOD_DEFAULT, 0, &awp_distance);



	ssystem->createSound(".\\res\\sounds\\rifle_reload.wav", FMOD_DEFAULT, 0, &rifle_reload);
	ssystem->createSound(".\\res\\sounds\\lmg_reload.ogg", FMOD_DEFAULT, 0, &lmg_reload);
	ssystem->createSound(".\\res\\sounds\\sniper_reload.ogg", FMOD_DEFAULT, 0, &sniper_reload);

	ssystem->createSound(".\\res\\sounds\\sniper_bolt.mp3", FMOD_DEFAULT, 0, &sniper_bolt);

	ssystem->createSound(".\\res\\sounds\\zoom.wav", FMOD_DEFAULT, 0, &zoom_sound);
	ssystem->createSound(".\\res\\sounds\\unzoom.wav", FMOD_DEFAULT, 0, &unzoom_sound);
	ssystem->createSound(".\\res\\sounds\\dry_fire.wav", FMOD_DEFAULT, 0, &dry_fire);

	ssystem->createSound(".\\res\\sounds\\hit.wav", FMOD_DEFAULT, 0, &hit_sound);
	ssystem->createSound(".\\res\\sounds\\walk.wav", FMOD_DEFAULT, 0, &walk);
	ssystem->createSound(".\\res\\sounds\\jump.wav", FMOD_DEFAULT, 0, &jump);
	ssystem->createSound(".\\res\\sounds\\land.wav", FMOD_DEFAULT, 0, &land_sound);
	ssystem->createSound(".\\res\\sounds\\exp_get.ogg", FMOD_DEFAULT, 0, &exp_get);
	ssystem->createSound(".\\res\\sounds\\hurt.mp3", FMOD_DEFAULT, 0, &hurt);
	ssystem->createSound(".\\res\\sounds\\player_dead.wav", FMOD_DEFAULT, 0, &dead);
	ssystem->createSound(".\\res\\sounds\\next_round.wav", FMOD_DEFAULT, 0, &next_round);

	ssystem->createSound(".\\res\\sounds\\monster_sound1.ogg", FMOD_DEFAULT, 0, &mst_idle_sound1);
	ssystem->createSound(".\\res\\sounds\\monster_sound2.ogg", FMOD_DEFAULT, 0, &mst_idle_sound2);

	ssystem->createSound(".\\res\\sounds\\monster_attack1.ogg", FMOD_DEFAULT, 0, &mst_attack_sound1);
	ssystem->createSound(".\\res\\sounds\\monster_attack2.ogg", FMOD_DEFAULT, 0, &mst_attack_sound2);
	ssystem->createSound(".\\res\\sounds\\button.wav", FMOD_DEFAULT, 0, &button_sound);
	ssystem->createSound(".\\res\\sounds\\weapon_button.wav", FMOD_DEFAULT, 0, &weapon_button);
	ssystem->createSound(".\\res\\sounds\\weapon_select.wav", FMOD_DEFAULT, 0, &weapon_select);
	ssystem->createSound(".\\res\\sounds\\start_button.wav", FMOD_DEFAULT, 0, &start_button);
	ssystem->createSound(".\\res\\sounds\\quit_button.wav", FMOD_DEFAULT, 0, &quit_button);

	ssystem->createSound(".\\res\\sounds\\cat_hit_ground.wav", FMOD_DEFAULT, 0, &cat_hit_ground);
	ssystem->createSound(".\\res\\sounds\\cat_stop.wav", FMOD_DEFAULT, 0, &cat_stop);

	ssystem->createSound(".\\res\\sounds\\paused.wav", FMOD_DEFAULT, 0, &pause);
	ssystem->createSound(".\\res\\sounds\\resume.wav", FMOD_DEFAULT, 0, &resume);
	ssystem->createSound(".\\res\\sounds\\explode.wav", FMOD_DEFAULT, 0, &ex_sound);
	ssystem->createSound(".\\res\\sounds\\throw.wav", FMOD_DEFAULT, 0, &pin_sound);
}

//몬스터 공격 사운드
void play_attack_sound() {
	std::random_device rd_sound; std::mt19937 gen(rd_sound()); 
	std::uniform_int_distribution<int> rand_sound(0, 1);
	ch_mst_attack_sound->stop();
	if (rand_sound(gen) == 0) ssystem->playSound(mst_attack_sound1, 0, false, &ch_mst_attack_sound); 
	else if (rand_sound(gen) == 1) ssystem->playSound(mst_attack_sound2, 0, false, &ch_mst_attack_sound); 
}

//몬스터 idle 사운드
void play_idle_sound() {
	std::random_device rd_sound; std::mt19937 gen(rd_sound());
	//지상형 몬스터가 1마리 이상 있을 경우 랜덤으로 소리 2가지 중 하나를 재생한다.
	if (mdx_r > 0 || mdx_big > 0) {
		if (monster_sound_delay < 300) monster_sound_delay++;
		if (monster_sound_delay == 300) {
			monster_sound_delay = 0;
			std::uniform_int_distribution<int> rand_sound(0, 1);
			ch_mst_idle_sound->stop();
			if (rand_sound(gen) == 0) ssystem->playSound(mst_idle_sound1, 0, false, &ch_mst_idle_sound);
			else if (rand_sound(gen) == 1) ssystem->playSound(mst_idle_sound2, 0, false, &ch_mst_idle_sound);
		}
	}
}

//버튼 사운드
void play_button_sound() {
	//일반 버튼 클릭
	if (button_feed_clickScene == TRUE) {
		ch_button->stop(); ssystem->playSound(button_sound, 0, false, &ch_button); 
		button_feed_clickScene = FALSE;
	}
	//총기 버튼 클릭
	if (button_feed_armory_button == TRUE) {
		ch_button->stop(); ssystem->playSound(weapon_button, 0, false, &ch_button); 
		button_feed_armory_button = FALSE;
	}
	//총기 선택 클릭
	if (button_feed_armory_select == TRUE) {
		ch_button->stop(); ssystem->playSound(weapon_select, 0, false, &ch_button); 
		button_feed_armory_select = FALSE;
	}
	//시작 버튼 클릭
	if (button_feed_clickScene_start == TRUE) {
		ch_button->stop(); ssystem->playSound(start_button, 0, false, &ch_button); 
		button_feed_clickScene_start = FALSE;
	}
	//나가기 버튼 클릭
	if (button_feed_clickScene_quit == TRUE) {
		ch_button->stop(); ssystem->playSound(quit_button, 0, false, &ch_button); 
		button_feed_clickScene_quit = FALSE;
	}
	//일시정지 사운드
	if (to_pause == TRUE) {
		ch_button->stop(); ssystem->playSound(pause, 0, false, &ch_button); 
		to_pause = FALSE;
	}
	//계속하기 사운드
	if (to_resume == TRUE) {
		ch_button->stop();ssystem->playSound(resume, 0, false, &ch_button); 
		to_resume = FALSE;
	}
}

//광클 방지
void mouse_fastClick_prevention() {
	if (after_delay < Gun::shoot_speed(GUN_number))	after_delay++; 
	if (after_delay == Gun::shoot_speed(GUN_number)) {
		can_shoot = TRUE; after_delay = 0;
	}
}

//조준점 출력
void show_target(HDC mdc, int mouse_x, int mouse_y, double var) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 12, RGB(0, 0, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, mouse_x + 12 + var, mouse_y, NULL);
	LineTo(mdc, mouse_x + 41 + var, mouse_y);
	MoveToEx(mdc, mouse_x - 12 - var, mouse_y, NULL);
	LineTo(mdc, mouse_x - 41 - var, mouse_y);
	MoveToEx(mdc, mouse_x, mouse_y - 12 - var, NULL);
	LineTo(mdc, mouse_x, mouse_y - 41 - var);
	MoveToEx(mdc, mouse_x, mouse_y + 12 + var, NULL);
	LineTo(mdc, mouse_x, mouse_y + 41 + var);

	SelectObject(mdc, oldpen); DeleteObject(hpen);
	 
	hpen = CreatePen(PS_SOLID, 5, RGB(40, 40, 40));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	//awp일 경우 관통 범위 표시
	if (GUN_number == awp) {
		MoveToEx(mdc, mouse_x - 500, mouse_y - 40, NULL);
		LineTo(mdc, mouse_x - 500, mouse_y + 40); 
		MoveToEx(mdc, mouse_x + 500, mouse_y - 40, NULL);
		LineTo(mdc, mouse_x + 500, mouse_y + 40);
	} 
	SelectObject(mdc, oldpen); DeleteObject(hpen);
	 
	hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, mouse_x + 13 + var, mouse_y, NULL);
	LineTo(mdc, mouse_x + 40 + var, mouse_y);
	MoveToEx(mdc, mouse_x - 13 - var, mouse_y, NULL);
	LineTo(mdc, mouse_x - 40 - var, mouse_y);
	MoveToEx(mdc, mouse_x, mouse_y - 13 - var, NULL);
	LineTo(mdc, mouse_x, mouse_y - 40 - var);
	MoveToEx(mdc, mouse_x, mouse_y + 13 + var, NULL);
	LineTo(mdc, mouse_x, mouse_y + 40 + var);

	SelectObject(mdc, oldpen); DeleteObject(hpen); 
}

//히트 포인트 표시
void show_hit(HDC mdc, int hit_x, int hit_y) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, hit_x - 5, hit_y - 5, NULL);
	LineTo(mdc, hit_x - 15, hit_y - 15);
	MoveToEx(mdc, hit_x + 5, hit_y - 5, NULL);
	LineTo(mdc, hit_x + 15, hit_y - 15);
	MoveToEx(mdc, hit_x - 5, hit_y + 5, NULL);
	LineTo(mdc, hit_x - 15, hit_y + 15);
	MoveToEx(mdc, hit_x + 5, hit_y + 5, NULL);
	LineTo(mdc, hit_x + 15, hit_y + 15);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

//경험치 획득 애니메이션 초기화
void init_exp_animation() {
	exp_x = -150, exp_acc = 20; out = 1; exp_out_delay = 0;
}

//정조준 완료 표시
void show_zoom_complited(HDC mdc) {
	zoom_complited.Draw(mdc, CM_x - 25 + ss_x, CM_y - ypos_zc + ss_y + landing_shake, 150, 150, 0, 0, 100, 100);
}

//awp 관통 대상 탐색
void find_target(int mx, int my) {
	//마우스 좌표 mx +- 500 범위 내에 몬스터가 있을 시 관통 대상이 된다. 
	for (int i = 0; i < mdx_r; i++) {
		if (CM_img_dir == 0 && mst_r[i].x <= mx && mst_r[i].x >= mx - 500 && my >= mst_r[i].y && my <= mst_r[i].y + 100) mst_r[i].targeted = 1; 
		else if (CM_img_dir == 1 && mst_r[i].x + 100 >= mx && mst_r[i].x + 100 <= mx + 500 && my >= mst_r[i].y && my <= mst_r[i].y + 100) mst_r[i].targeted = 1; 
		else mst_r[i].targeted = 0; 	
	} 
	for (int i = 0; i < mdx_big; i++) {
		if (CM_img_dir == 0 && mst_big[i].x <= mx && mst_big[i].x >= mx - 500 && my >= mst_big[i].y && my <= mst_big[i].y + 200) mst_big[i].targeted = 1; 
		else if (CM_img_dir == 1 && mst_big[i].x + 200 >= mx && mst_big[i].x + 200 <= mx + 500 && my >= mst_big[i].y && my <= mst_big[i].y + 200) mst_big[i].targeted = 1; 
		else mst_big[i].targeted = 0; 	
	}
}

//awp 관통 대상 표시
void show_awp_targeted(HDC mdc) {
	//관통 대상이 된 몬스터는 따로 표시된다.
	for (int i = 0; i < mdx_r; i++) {
		if (mst_r[i].targeted == 1) zoom_targeted.Draw(mdc, mst_r[i].x + ss_x, mst_r[i].y - 90 + ss_x + landing_shake, 100, 100, 0, 0, 100, 100); 
	} 
	for (int i = 0; i < mdx_big; i++) {
		if (mst_big[i].targeted == 1) zoom_targeted.Draw(mdc, mst_big[i].x + 50 + ss_x, mst_big[i].y - 90 + landing_shake, 100, 100, 0, 0, 100, 100); 
	}
}

//인터페이스 출력
void show_interface(HDC mdc, RECT rt) {
	//장탄수 표시기 배경
	IND_w = indicator_back.GetWidth(); IND_h = indicator_back.GetHeight();
	indicator_back.Draw(mdc, rt.right - 600 + ss_x, rt.bottom - 110 + landing_shake + ss_y, 600, 110, 0, 0, IND_w, IND_h);

	//수류탄 대기 시간 출력
	show_gren_time(mdc, ss_x, ss_y, landing_shake, gren_time);
	if (able_grenade == TRUE) avail_grenade.Draw(mdc, 420 + ss_x, 680 + ss_y + landing_shake, 80, 80, 0, 0, 100, 100); 
	if(able_grenade == FALSE) unavail_grenade.Draw(mdc, 420 + ss_x, 680 + ss_y + landing_shake, 80, 80, 0, 0, 100, 100);
		 
	//총알 아이콘
	if (GUN_number == scar_h || GUN_number == m16 || GUN_number == mp_44) {
		AMO_w = ammo_icon.GetWidth(); AMO_h = ammo_icon.GetHeight();
		ammo_icon.Draw(mdc, rt.right - 260 + ss_x, rt.bottom - 108 + landing_shake + ss_y, 100, 100, 0, 0, AMO_w, AMO_h);
	} 
	if (GUN_number == mg_42) {
		AMO_w = ammo_lmg_icon.GetWidth(); AMO_h = ammo_lmg_icon.GetHeight();
		ammo_lmg_icon.Draw(mdc, rt.right - 230 + ss_x, rt.bottom - 108 + landing_shake + ss_y, 100, 100, 0, 0, AMO_w, AMO_h);
	} 
	if (GUN_number == awp) {
		AMO_w = ammo_sniper_icon.GetWidth(); AMO_h = ammo_lmg_icon.GetHeight();
		ammo_sniper_icon.Draw(mdc, rt.right - 200 + ss_x, rt.bottom - 105 + landing_shake + ss_y, 100, 100, 0, 0, AMO_w, AMO_h);
	}
	 
	//총 아이콘 및 장탄수 출력
	switch (GUN_number) {
	case scar_h:
		GUN_w = SCAR_H_right.GetWidth(); GUN_h = SCAR_H_right.GetWidth();
		SCAR_H_right.Draw(mdc, rt.right - 430 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 150, 150, 0, 0, GUN_w, GUN_h);
		break; 
	case m16:
		GUN_w = M16_right.GetWidth(); GUN_h = M16_right.GetWidth();
		M16_right.Draw(mdc, rt.right - 430 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 150, 150, 0, 0, GUN_w, GUN_h);
		break; 
	case mp_44:
		GUN_w = MP44_right.GetWidth(); GUN_h = MP44_right.GetWidth();
		MP44_right.Draw(mdc, rt.right - 430 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 150, 150, 0, 0, GUN_w, GUN_h);
		break; 
	case mg_42:
		GUN_w = MG42_right.GetWidth(); GUN_h = MG42_right.GetHeight();
		MG42_right.Draw(mdc, rt.right - 500 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 250, 150, 0, 0, GUN_w, GUN_h);
		break; 
	case awp:
		GUN_w = AWP_right.GetWidth(); GUN_h = AWP_right.GetHeight();
		AWP_right.Draw(mdc, rt.right - 450 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 200, 150, 0, 0, GUN_w, GUN_h);
		break;
	}
	//mdc 오른쪽에 최대 장탄수, 그 오른쪽에 현재 장탄수 입력
	ammo_indicator(mdc, Gun::max_ammo(GUN_number), ammo, ind_size, ind_x + ss_x, ind_y + landing_shake + ss_y);
	 
	//경험치 수치 출력
	show_exp(mdc, experience, rt.left + 130 + ss_x, rt.top + 3 + ss_y + landing_shake);
	exp_icon.Draw(mdc, rt.left + 20 + ss_x, rt.top + 15 + ss_y + landing_shake, 100, 50, 0, 0, 100, 50);
	show_exp_add(mdc, prev_up, exp_x + ss_x, rt.top + 70 + ss_y + landing_shake);
	 
	//재장전 게이지 출력
	if (reload == 1)
		reload_indicator(mdc, CM_x + ss_x, CM_y - 30 + landing_shake + ss_y, CM_x + reload_x + ss_x, CM_y - 10 + landing_shake + ss_y, CM_x + ss_x, CM_y - 30 + landing_shake + ss_y, CM_x + 100 + ss_x, CM_y - 10 + landing_shake + ss_y);
	 
	//몬스터 체력 게이지 출력
	for (int i = 0; i < mdx_r; i++)
		monster_hp_ind(mdc, mst_r[i].x + ss_x, mst_r[i].y - 30 + landing_shake + ss_y, mst_r[i].x + mst_r[i].hp * 2 + ss_x, mst_r[i].y - 15 + landing_shake + ss_y,
			mst_r[i].x + ss_x, mst_r[i].y - 30 + landing_shake + ss_y, mst_r[i].x + 100 + ss_x, mst_r[i].y - 15 + landing_shake + ss_y);
	//대형 몬스터
	for (int i = 0; i < mdx_big; i++)
		monster_hp_ind(mdc, mst_big[i].x + ss_x, mst_big[i].y - 30 + landing_shake + ss_y, mst_big[i].x + mst_big[i].hp * 2 + ss_x, mst_big[i].y - 15 + landing_shake + ss_y,
			mst_big[i].x + ss_x, mst_big[i].y - 30 + landing_shake + ss_y, mst_big[i].x + 200 + ss_x, mst_big[i].y - 15 + landing_shake + ss_y);
	//공중 몬스터
	for (int i = 0; i < mdx_air; i++)
		monster_hp_ind(mdc, mst_air[i].x + ss_x, mst_air[i].y - 30 + landing_shake + ss_y, mst_air[i].x + mst_air[i].hp * 5 + ss_x, mst_air[i].y - 15 + landing_shake + ss_y,
			mst_air[i].x + ss_x, mst_air[i].y - 30 + landing_shake + ss_y, mst_air[i].x + 150 + ss_x, mst_air[i].y - 15 + landing_shake + ss_y);
	//플레이어 체력, 따로 만들기 귀찮아서 몬스터 체력 게이지로 공용
	monster_hp_ind(mdc, (rt.left + 10) + ss_x, (rt.bottom - 30) + landing_shake + ss_y, (rt.left + 10) + (health * 3) + ss_x, (rt.bottom - 10) + landing_shake + ss_y,
	(rt.left + 10) + ss_x, (rt.bottom - 30) + landing_shake + ss_y, (rt.left + 10) + 300 + ss_x, (rt.bottom - 10) + landing_shake + ss_y);

	//플레이어 체력 숫자 표시
	player_health(mdc, rt, ss_x, ss_y, landing_shake, health);
}

//플레이어 이미지, 총 이미지 출력
void show_player(HDC mdc) {
	switch (CM_img_dir) { //플레이어, 총 이미지 출력
	case 0:
		//플레이어 이미지 좌픅
		if (CM_jump == 0) {
			//awp 정조준 시 스코프를 바라보는 모습을 취함
			if (is_zoom == TRUE) {
				CM_w = commando_zoom_left.GetWidth(); CM_h = commando_zoom_left.GetHeight();
				commando_zoom_left.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h);
			} 
			else {
				if (is_draw == TRUE) {
					CM_w = commando_fire_left.GetWidth(); CM_h = commando_fire_left.GetHeight();
					commando_fire_left.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
				}
				else if (is_draw == FALSE) {
					CM_w = commando_left.GetWidth(); CM_h = commando_left.GetHeight();
					commando_left.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
				}
			}
		} 
		else if (CM_jump == 1 || CM_jump == 2) {
			if (is_draw == TRUE) {
				CM_w = commando_jump_fire_left.GetWidth(); CM_h = commando_jump_fire_left.GetHeight();
				commando_jump_fire_left.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
			}
			else if (is_draw == FALSE) {
				CM_w = commando_jump_left.GetWidth(); CM_h = commando_jump_left.GetHeight();
				commando_jump_left.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
			}
		}

		//총 좌측
		switch (GUN_number) {
		case scar_h:
			GUN_w = SCAR_H_left.GetWidth(); GUN_h = SCAR_H_left.GetHeight();
			SCAR_H_left.Draw(mdc, CM_x - 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break; 
		case m16:
			GUN_w = M16_left.GetWidth(); GUN_h = M16_left.GetHeight();
			M16_left.Draw(mdc, CM_x - 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break; 
		case mp_44:
			GUN_w = MP44_left.GetWidth(); GUN_h = MP44_left.GetHeight();
			MP44_left.Draw(mdc, CM_x - 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break; 
		case mg_42:
			GUN_w = MG42_left.GetWidth(); GUN_h = MG42_left.GetHeight();
			MG42_left.Draw(mdc, CM_x - 120 + ss_x, CM_y + landing_shake + ss_y, 200, 100, 0, 0, GUN_w, GUN_h);
			break; 
		case awp:
			GUN_w = AWP_left.GetWidth(); GUN_h = AWP_left.GetHeight();
			AWP_left.Draw(mdc, CM_x - 80 +ss_x, CM_y + landing_shake + ss_y, 150, 100, 0, 0, GUN_w, GUN_h);
			break;
		}

		//불꽃 좌측
		if (is_draw == TRUE && GUN_number != mg_42 && GUN_number != awp)
			flame_left.Draw(mdc, CM_x - 140 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, 100, 100); 
		if (is_draw == TRUE && GUN_number == mg_42)
			flame_left.Draw(mdc, CM_x - 220 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, 100, 100); 
		if (is_draw == TRUE && GUN_number == awp)
			flame_left.Draw(mdc, CM_x - 170 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, 100, 100);

		break;
		//////////////////////
	case 1:
		//플레이어 우측
		if (CM_jump == 0) {
			if (is_zoom == TRUE) {
				CM_w = commando_zoom_right.GetWidth(); CM_h = commando_zoom_right.GetHeight();
				commando_zoom_right.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h);
			}
			else {
				if (is_draw == TRUE) {
					CM_w = commando_fire_right.GetWidth(); CM_h = commando_fire_right.GetHeight();
					commando_fire_right.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
				}
				else if (is_draw == FALSE) {
					CM_w = commando_right.GetWidth(); CM_h = commando_right.GetHeight();
					commando_right.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
				}
			}
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			if (is_draw == TRUE) {
				CM_w = commando_jump_fire_right.GetWidth(); CM_h = commando_jump_fire_right.GetHeight();
				commando_jump_fire_right.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
			}
			else if (is_draw == FALSE) {
				CM_w = commando_jump_right.GetWidth(); CM_h = commando_jump_right.GetHeight();
				commando_jump_right.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
			}
		}

		//총 우측
		switch (GUN_number) {
		case scar_h:
			GUN_w = SCAR_H_right.GetWidth(); GUN_h = SCAR_H_right.GetHeight();
			SCAR_H_right.Draw(mdc, CM_x + 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h);
			break;
		case m16:
			GUN_w = M16_right.GetWidth(); GUN_h = M16_right.GetHeight();
			M16_right.Draw(mdc, CM_x + 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break;
		case mp_44:
			GUN_w = MP44_right.GetWidth(); GUN_h = MP44_right.GetHeight();
			MP44_right.Draw(mdc, CM_x + 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break;
		case mg_42:
			GUN_w = MG42_right.GetWidth();  GUN_h = MG42_left.GetHeight();
			MG42_right.Draw(mdc, CM_x + 20 + ss_x, CM_y + landing_shake + ss_y, 200, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break;
		case awp:
			GUN_w = AWP_right.GetWidth(); GUN_h = AWP_right.GetHeight();
			AWP_right.Draw(mdc, CM_x + 30 + ss_x, CM_y + landing_shake + ss_y, 150, 100, 0, 0, GUN_w, GUN_h);
			break;
		}

		//불꽃 우측
		if (is_draw == TRUE && GUN_number != mg_42 && GUN_number != awp)
			flame_right.Draw(mdc, CM_x + 140 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, 100, 100);

		if(is_draw == TRUE && GUN_number == mg_42)
			flame_right.Draw(mdc, CM_x + 220 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, 100, 100);

		if (is_draw == TRUE && GUN_number == awp)
			flame_right.Draw(mdc, CM_x + 170 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, 100, 100);

		break;
	}
}

//몬스터 생성
void make_monster(RECT rt) {
	//spawn_timer의 수치가 0이 되면 몬스터가 한 마리씩 스폰
	//일반 몬스터
	spawn_timer_r--;
	if (spawn_timer_r == 0) {
		spawn_timer_r = calc_spawn_timer(spawn_timer_r_max);		// 원래값 500
		if (mdx_r < 99) {
			spawn_monster_regular(mdx_r, BG_scanner, rt); mdx_r++;
		}
	}  
	//대형 몬스터
	spawn_timer_big--;
	if (spawn_timer_big == 0) {
		spawn_timer_big = calc_spawn_timer(spawn_timer_big_max);	// 원래값 1000
		if (mdx_big < 99) {
			spawn_monster_big(mdx_big, BG_scanner, rt); mdx_big++;
		}
	} 
	//공중 몬스터
	spawn_timer_air--;
	if (spawn_timer_air == 0) {
		spawn_timer_air = calc_spawn_timer(spawn_timer_air_max);	// 원래값 600
		if (mdx_air < 99) {
			spawn_monster_air(mdx_air, BG_scanner, rt); mdx_air++;
		}
	}
}

//몬스터 이미지 출력
void show_monster(HDC mdc, int ss_x, int ss_y, int landing_shake) {
	//죽은 몬스터 출력
	for (int i = 0; i < ddx; i++) {
		if (dl[i].monster_type == 1) {
			if(dl[i].dir == 0) monster_dead_left.Draw(mdc, dl[i].x + ss_x, dl[i].y + ss_y + landing_shake, 100, 100, 0, 0, 100, 100); 
			else if(dl[i].dir == 1) monster_dead_right.Draw(mdc, dl[i].x + ss_x, dl[i].y + ss_y + landing_shake, 100, 100, 0, 0, 100, 100); 
		}
		else if (dl[i].monster_type == 2) {
			if(dl[i].dir == 0) monster_big_dead_left.Draw(mdc, dl[i].x + ss_x, dl[i].y + 50 + ss_y + landing_shake, 200, 200, 0, 0, 200, 200); 
			else if (dl[i].dir == 1) monster_big_dead_right.Draw(mdc, dl[i].x + ss_x, dl[i].y + 50 + ss_y + landing_shake, 200, 200, 0, 0, 200, 200); 
		}
		else if (dl[i].monster_type == 3) monster_air_dead.Draw(mdc, dl[i].x + ss_x, dl[i].y  + ss_y + landing_shake, 150, 60, 0, 0, 150, 60); 
	} 
	//일반 몬스터 출력
	for (int i = 0; i < mdx_r; i++) {
		switch (mst_r[i].img_dir) {
		case 0:
			MST_w = monster_left.GetWidth(); MST_h = monster_left.GetHeight(); 
			monster_left.Draw(mdc, mst_r[i].x + ss_x, mst_r[i].y + ss_y + landing_shake, 100, mst_r[i].height, 0, 0, MST_w, MST_h);
			break; 
		case 1:
			MST_w = monster_right.GetWidth(); MST_h = monster_right.GetHeight(); 
			monster_right.Draw(mdc, mst_r[i].x + ss_x, mst_r[i].y + ss_y + landing_shake, 100, mst_r[i].height, 0, 0, MST_w, MST_h);
			break;
		}
	} 
	//대형 몬스터 출력
	for (int i = 0; i < mdx_big; i++) {
		switch (mst_big[i].img_dir) {
		case 0:
			MST_big_w = monster_big_left.GetWidth(); MST_big_h = monster_big_left.GetHeight(); 
			monster_big_left.Draw(mdc, mst_big[i].x + ss_x, mst_big[i].y + ss_y + landing_shake, 200, mst_big[i].height, 0, 0, MST_big_w, MST_big_h);
			break; 
		case 1:
			MST_big_w = monster_big_right.GetWidth(); MST_big_h = monster_big_right.GetHeight(); 
			monster_big_right.Draw(mdc, mst_big[i].x + ss_x, mst_big[i].y + ss_y + landing_shake, 200, mst_big[i].height , 0, 0, MST_big_w, MST_big_h);
			break;
		}
	} 
	//공중 몬스터 출력
	for (int i = 0; i < mdx_air; i++) {
		switch (mst_air[i].img_dir) {
		case 0:
			MST_air_w = monster_air_left[air].GetWidth(); MST_air_h = monster_air_left[air].GetHeight(); 
			monster_air_left[air].Draw(mdc, mst_air[i].x + ss_x, mst_air[i].y + ss_y + landing_shake, 150, mst_air[i].height, 0, 0, MST_air_w, MST_air_h);
			break; 
		case 1:
			MST_air_w = monster_air_right[air].GetWidth(); MST_air_h = monster_air_right[air].GetHeight(); 
			monster_air_right[air].Draw(mdc, mst_air[i].x + ss_x, mst_air[i].y + ss_y + landing_shake, 150, mst_air[i].height, 0, 0, MST_air_w, MST_air_h);
			break;
		}
	}
}

//탄피 출력
void show_catridge(HDC mdc, int ss_x, int ss_y, int landing_shake) {
	for (int i = cdx - 1; i >= 0; i--)
		catridge[gc[i].frame].Draw(mdc, gc[i].x + ss_x, gc[i].y + ss_y + landing_shake, 20, 20, 0, 0, 20, 20);
}

//수류탄 출력
void show_grenade(HDC mdc, int ss_x, int ss_y, int landing_shake) {
	if(is_throw == TRUE || (set_grenade == TRUE && is_boom == FALSE))
		grenade[g_frame].Draw(mdc, gren_x + ss_x, gren_y + ss_y + landing_shake, 60, 60, 0, 0, 60, 60);
	if (is_boom == TRUE)
		explode[ex_frame].Draw(mdc, (gren_x - 180) + ss_x, (gren_y - 250) + ss_y + landing_shake, 400, 400, 0, 0, 100, 100);
}

//플레이어 이미지 방향 업데이트
void update_player_direction(int mouse_x) {
	if(mouse_x < CM_x + 50) CM_img_dir = 0; 
	if(mouse_x > CM_x + 50) CM_img_dir = 1; 
	//마우스 좌표가 플레이어보다 왼쪽에 있으면 왼쪽을 바라보고, 오른쪽에 있으면 오른쪽을 바라봄
}

//플레이어 위치 업데이트
void update_player_position(RECT rt) {
//점프
	if (CM_jump == 1) { //위로 올라가는 중
		CM_y -= CM_jump_acc; CM_jump_acc--; //위로 올라갈수록 가속이 줄어듬
		if (CM_jump_acc == -1) {            //가속이 완전히 줄어들면
			CM_jump_acc = 0; CM_jump = 2;   //떨어지기 시작 
		}
	}
	else if (CM_jump == 2) { //떨어지는 중
		CM_y += CM_jump_acc; CM_jump_acc++;                //떨어지면서 가속이 증가함
		if (CM_jump_acc == 29) { //땅에 닿으면
			CM_jump_acc = 28; CM_jump = 0; space_pressed = 0; //플레이어는 땅에 착지하고 VK_SPACE 입력을 받을 준비를 함
			is_land = 1;
			ch_land->stop(); ssystem->playSound(land_sound, 0, false, &ch_land); //사운드 재생 
		}
	} 
	if (is_land == 1) { //점프 후 착지 시 화면에 흔들림 발생
		if (shake_reduce == 0) {
			landing_shake = 30; shake_reduce = 1; //화면 전체가 30씩 내려간다.
		}
		if (shake_reduce == 1) {
			landing_shake -= 2;                   //시간이 지나면서 점차 원래대로 돌아온다
			if (landing_shake == 0) {
				is_land = 0; shake_reduce = 0; 
			}
		}
	} 
//좌우 이동
	if (CM_move_dir == 0) { //좌측 이동
		if (BG_scanner >= 10 && CM_x == 700) {  //배경 인식 좌표가 10보다 크고 플레이어 x 좌표가 700이면 배경이 움직임
			for (int i = 0; i < mdx_r; i++)	mst_r[i].x += 15; //나머지 모든 오브젝트들도 따라 움직임
			for (int i = 0; i < mdx_big; i++) mst_big[i].x += 15; 
			for (int i = 0; i < mdx_air; i++) mst_air[i].x += 15; 
			for (int i = 0; i < ddx; i++) dl[i].x += 15; 
			for (int i = 0; i < cdx; i++) gc[i].x += 15; 
			gren_x += 15; BG_scanner -= 15;
		} 
		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2990 && CM_x >= 700)) CM_x -= 15;   
			//배경 인식 좌표가 10이되고 플레이어가 다시 가운데로 이동할 때까지 플레이어만 움직임 
		if (CM_x <= rt.left) CM_x += 15;  
			//벽에 닿으면 이동 중지      
	} 
	else if (CM_move_dir == 1) { //우측 이동
		if (BG_scanner <= 2990 && CM_x == 700) { 
			for (int i = 0; i < mdx_r; i++) mst_r[i].x -= 15; 
			for (int i = 0; i < mdx_big; i++) mst_big[i].x -= 15; 
			for (int i = 0; i < mdx_air; i++) mst_air[i].x -= 15; 
			for (int i = 0; i < ddx; i++) dl[i].x -= 15; 
			for (int i = 0; i < cdx; i++) gc[i].x -= 15; 
			gren_x -= 15; BG_scanner += 15;
		} 
		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2990 && CM_x >= 700)) CM_x += 15; 
		if (CM_x + 100 >= rt.right) CM_x -= 15; 
	} 
}

//몬스터 이미지 방향 업데이트
void update_monster_direction(double CM_x) {
	//일반 몬스터
	for (int i = mdx_r - 1; i >= 0; i--) update_monster_dir_r(mst_r[i].x + ss_x, CM_x + ss_x, i); 
	//대형 몬스터
	for (int i = mdx_big - 1; i >= 0; i--) update_monster_dir_big(mst_big[i].x + ss_x, CM_x + ss_x, i); 
	//공중 몬스터
	for (int i = mdx_air - 1; i >= 0; i--) update_monster_dir_air(mst_air[i].x + ss_x, CM_x + ss_x, i); 
}

//몬스터 위치 업데이트
void update_monster_position() {
	//일반 몬스터 위치
	for (int i = mdx_r - 1; i >= 0; i--) {
		if (mst_r[i].x > CM_x + 4 || mst_r[i].x < CM_x - 4) {
			if (mst_r[i].img_dir == 0) mst_r[i].x -= 5; 
			else if (mst_r[i].img_dir == 1) mst_r[i].x += 5; 
		}
	} 
	//대형 몬스터 위치
	for (int i = mdx_big - 1; i >= 0; i--) {
		if (mst_big[i].x + 50 > CM_x + 4 || mst_big[i].x + 50 < CM_x - 4) {
			if (mst_big[i].img_dir == 0) mst_big[i].x -= 3; 
			else if (mst_big[i].img_dir == 1) mst_big[i].x += 3;
		}
	} 
	//공중 몬스터 위치
	for (int i = mdx_air - 1; i >= 0; i--) {
		if (mst_air[i].x + 25 > CM_x + 4 || mst_air[i].x + 25 < CM_x - 4) {
			if (mst_air[i].img_dir == 0) mst_air[i].x -= 2; 
			else if (mst_air[i].img_dir == 1) mst_air[i].x += 2; 
		}
	}
}

//AWP 전용 몬스터 명중 판정
void check_hit_awp() {
	//조건문 중복 방지 변수
	int is_kill_r = 0; int is_kill_big = 0; int is_kill_air = 0;
	//최소 한 마리의 몬스터를 맞추어야 관통 효과가 발동된다
	for (int i = mdx_r - 1; i >= 0; i--) {
		if (hit_x >= mst_r[i].x && hit_x <= mst_r[i].x + 100 && hit_y >= mst_r[i].y && hit_y <= mst_r[i].y + 100) {
			ch_hit->stop(); ssystem->playSound(hit_sound, 0, false, &ch_hit); //사운드 재생
			is_hit = TRUE;
		}
	}

	for (int i = mdx_big - 1; i >= 0; i--) {
		if (hit_x >= mst_big[i].x && hit_x <= mst_big[i].x + 200 && hit_y >= mst_big[i].y && hit_y <= mst_big[i].y + 200) {
			ch_hit->stop(); ssystem->playSound(hit_sound, 0, false, &ch_hit); //사운드 재생
			is_hit = TRUE;
		}
	} 
	//관통 대상이 된 몬스터들은 모두 대미지를 입는다
	for (int i = mdx_r - 1; i >= 0; i--) {
		if (is_hit == TRUE && mst_r[i].targeted == 1) {
			mst_r[i].hp = cal_damage(mst_r[i].hp, GUN_number);
			if (mst_r[i].hp <= 0) {
				if (i < mdx_r - 1 && is_kill_r == 0) {
					dl[ddx].x = mst_r[i].x; dl[ddx].y = mst_r[i].y; dl[ddx].monster_type = 1; dl[ddx].dir = mst_r[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					monster_array_push_r(i, mdx_r--); experience += 5; prev_up = 5; exp_up = TRUE;
					init_exp_animation(); is_kill_r = 1; kill_count++;
				}
				else if (i == mdx_r - 1 && is_kill_r == 0) {
					dl[ddx].x = mst_r[i].x; dl[ddx].y = mst_r[i].y; dl[ddx].monster_type = 1; dl[ddx].dir = mst_r[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					mdx_r--; experience += 5; prev_up = 5; exp_up = TRUE;
					init_exp_animation(); is_kill_r = 1; kill_count++;
				}
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp);
				is_kill_r = 0;
			}
		}
	} 
	for (int i = mdx_big - 1; i >= 0; i--) {
		if (is_hit == TRUE && mst_big[i].targeted == 1) {
			mst_big[i].hp = cal_damage(mst_big[i].hp, GUN_number);
			if (mst_big[i].hp <= 0) {
				if (i < mdx_big - 1 && is_kill_big == 0) {
					dl[ddx].x = mst_big[i].x; dl[ddx].y = mst_big[i].y; dl[ddx].monster_type = 2; dl[ddx].dir = mst_big[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					monster_array_push_big(i, mdx_big--); experience += 7; prev_up = 7; exp_up = TRUE;
					init_exp_animation(); is_kill_big = 1; kill_count++;
				}
				else if (i == mdx_big - 1 && is_kill_big == 0) {
					dl[ddx].x = mst_big[i].x; dl[ddx].y = mst_big[i].y; dl[ddx].monster_type = 2; dl[ddx].dir = mst_big[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					mdx_big--; experience += 7; prev_up = 7; exp_up = TRUE;
					init_exp_animation(); is_kill_big = 1; kill_count++;
				} 
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp); //사운드 재생
				is_kill_big = 0;
			}
		}
	} 
	//공중 몬스터 히트 판정 //관통 대상에서 제외
	for (int i = mdx_air - 1; i >= 0; i--) {
		if (hit_x >= mst_air[i].x && hit_x <= mst_air[i].x + 150 && hit_y >= mst_air[i].y && hit_y <= mst_air[i].y + 60) {
			hit = i;

			mst_air[hit].hp = cal_damage(mst_air[hit].hp, GUN_number);
			ch_hit->stop(); ssystem->playSound(hit_sound, 0, false, &ch_hit); //사운드 재생
			
			if (mst_air[hit].hp <= 0) {
				if (hit < mdx_air - 1 && is_kill_air == 0) {
					dl[ddx].x = mst_air[hit].x; dl[ddx].y = mst_air[hit].y; dl[ddx].monster_type = 3; dl[ddx++].acc = 0;
					monster_array_push_air(hit, mdx_air--); experience += 3; prev_up = 3; exp_up = TRUE;
					init_exp_animation(); is_kill_air = 1; kill_count++;
				}
				else if (hit == mdx_air - 1 && is_kill_air == 0) {
					dl[ddx].x = mst_air[hit].x; dl[ddx].y = mst_air[hit].y; dl[ddx].monster_type = 3; dl[ddx++].acc = 0;
					mdx_air--; experience += 3; prev_up = 3; exp_up = TRUE;
					init_exp_animation(); is_kill_air = 1; kill_count++;
				}
				ch_exp->stop();	ssystem->playSound(exp_get, 0, false, &ch_exp); //사운드 재생
				is_kill_air = 0;
			}
		}
	}
}

//몬스터 명중 판정
void check_hit() {
	//조건문 중복 방지 변수
	int is_kill = 0;
	//일반 몬스터 히트 판정
	for (int i = mdx_r - 1; i >= 0; i--) {	// 중간 몬스터가 삭제될시 인덱스가 하나 줄어서 마지막 몬스터를 검사하지 못하므로 반대로 돌립니다.
		if (hit_x >= mst_r[i].x && hit_x <= mst_r[i].x + 100 && hit_y >= mst_r[i].y && hit_y <= mst_r[i].y + 100) {
			hit = i;    //조준점 내부의 좌표가 몬스터 이미지 내부에 위치하면 히트로 판정되어 총알은 해당 몬스터에 가로막힌다.
			angle = atan2(hit_y - (CM_y + 60), hit_x - (CM_x + 50));
			ammo_x2 = ammo_x1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * cos(angle);
			ammo_y2 = ammo_y1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * sin(angle);

			mst_r[hit].hp = cal_damage(mst_r[hit].hp, GUN_number);
			ch_hit->stop(); ssystem->playSound(hit_sound, 0, false, &ch_hit); //사운드 재생
			
			//최신 인덱스는 그냥 인덱스를 감소, 중간 인덱스일 경우 해당 인덱스 데이터에 바로 다음 인덱스의 데이터를 덮어씌움(인덱스 밀어내기)
			if (mst_r[hit].hp <= 0) {
				if (hit < mdx_r - 1 && is_kill == 0) {
					//죽기 직전에 몬스터가 가지고 있던 위치, 방향 정보를 시체 구조체로 복사한다.
					dl[ddx].x = mst_r[hit].x; dl[ddx].y = mst_r[hit].y; dl[ddx].monster_type = 1; dl[ddx].dir = mst_r[hit].img_dir;  
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					monster_array_push_r(hit, mdx_r--); experience += 5; prev_up = 5; exp_up = TRUE;
					init_exp_animation(); is_kill = 1; kill_count++;
				}
				else if (hit == mdx_r - 1 && is_kill == 0) {
					dl[ddx].x = mst_r[hit].x; dl[ddx].y = mst_r[hit].y; dl[ddx].monster_type = 1; dl[ddx].dir = mst_r[hit].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					mdx_r--; experience += 5; prev_up = 5; exp_up = TRUE;
					init_exp_animation(); is_kill = 1; kill_count++;
				}
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp); //사운드 재생
				is_kill = 0;
			}
		}
	} 
	//대형 몬스터 히트 판정
	for (int i = mdx_big - 1; i >= 0; i --) {
		if (hit_x >= mst_big[i].x && hit_x <= mst_big[i].x + 200 && hit_y >= mst_big[i].y && hit_y <= mst_big[i].y + 200) {
			hit = i;
			angle = atan2(hit_y - (CM_y + 60), hit_x - (CM_x + 50));
			ammo_x2 = ammo_x1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * cos(angle);
			ammo_y2 = ammo_y1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * sin(angle);

			mst_big[hit].hp = cal_damage(mst_big[hit].hp, GUN_number);
			ch_hit->stop(); ssystem->playSound(hit_sound, 0, false, &ch_hit); //사운드 재생

			if (mst_big[hit].hp <= 0) {
				if (hit < mdx_big - 1 && is_kill == 0) {
					dl[ddx].x = mst_big[hit].x; dl[ddx].y = mst_big[hit].y; dl[ddx].monster_type = 2; dl[ddx].dir = mst_big[hit].img_dir;
					dl[ddx].acc = 20;  dl[ddx++].motion_dir = 1;
					monster_array_push_big(hit, mdx_big--); experience += 7; prev_up = 7; exp_up = TRUE;
					init_exp_animation(); is_kill = 1; kill_count++;
				}
				else if (hit == mdx_big - 1 && is_kill == 0) {
					dl[ddx].x = mst_big[hit].x; dl[ddx].y = mst_big[hit].y; dl[ddx].monster_type = 2; dl[ddx].dir = mst_big[hit].img_dir;
					dl[ddx].acc = 20;  dl[ddx++].motion_dir = 1;
					mdx_big--; experience += 7; prev_up = 7; exp_up = TRUE;
					init_exp_animation(); is_kill = 1; kill_count++;
				}
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp); //사운드 재생
				is_kill = 0;
			}
		}
	} 
	//공중 몬스터 히트 판정
	for (int i = mdx_air - 1; i >= 0; i --) {
		if (hit_x >= mst_air[i].x && hit_x <= mst_air[i].x + 150 && hit_y >= mst_air[i].y && hit_y <= mst_air[i].y + 60) {
			hit = i;
			angle = atan2(hit_y - (CM_y + 60), hit_x - (CM_x + 50));
			ammo_x2 = ammo_x1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * cos(angle);
			ammo_y2 = ammo_y1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * sin(angle);

			mst_air[hit].hp = cal_damage(mst_air[hit].hp, GUN_number);
			ch_hit->stop(); ssystem->playSound(hit_sound, 0, false, &ch_hit); //사운드 재생
		
			if (mst_air[hit].hp <= 0) {
				if (hit < mdx_air - 1 && is_kill == 0) {
					dl[ddx].x = mst_air[hit].x; dl[ddx].y = mst_air[hit].y; dl[ddx].monster_type = 3; dl[ddx++].acc = 0;
					monster_array_push_air(hit, mdx_air--); experience += 3; prev_up = 3; exp_up = TRUE;
					init_exp_animation(); is_kill = 1; kill_count++;
				}
				else if (hit == mdx_air - 1 && is_kill == 0) {
					dl[ddx].x = mst_air[hit].x; dl[ddx].y = mst_air[hit].y; dl[ddx].monster_type = 3; dl[ddx++].acc = 0;
					mdx_air--; experience += 3; prev_up = 3; exp_up = TRUE;
					init_exp_animation(); is_kill = 1; kill_count++;
				}
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp); //사운드 재생
				is_kill = 0;
			}
		}
	}
}

//사격 시 화면 흔들림 생성
void make_shake(int shake_acc, int shake_end) {
	std::random_device rd_shake; std::mt19937 gen(rd_shake()); 
	std::uniform_int_distribution<int> shake_x(-shake_acc, shake_acc); //숫자가 클 수록 반동이 커 화면 흔들림이 격해짐
	std::uniform_int_distribution<int> shake_y(-shake_acc, shake_acc); //짧은 순간에 ss_x, ss_y에 랜덤한 값을 넣어 흔들림 표현
	ss_x = shake_x(gen); ss_y = shake_y(gen);
	shake_count++;          //몇 번 카운트를 세냐에 따라 화면 흔들리는 시간이 달라진다. 많이 세면 샐 수록 흔들리는 시간이 길어진다.
	if (shake_count >= shake_end) { //특정 횟수가 되면
		shake_count = 0;    //카운트 초기화
		ss_x = 0; ss_y = 0; //화면 좌표는 원래대로 돌아온다
		shake_effect = 0;   //더 이상 흔들리지 않는다.
	}
}

//총알 발사 시 수행되는 작업 모음
void make_rand_ammo(int ammo, int max_ammo) {
	if (ammo + 1 == max_ammo) empty = 1;							//총알 소모가 정히진 값에 도달하면 더 이상 발사되지 않는다

	std::random_device rd_ammo; std::mt19937 gen(rd_ammo()); 
	std::uniform_int_distribution<int> x(mx - (10 + var), mx + (10 + var)); //분산도가 넓어질수록 정확도가 떨어지게됨
	std::uniform_int_distribution<int> y(my - (10 + var), my + (10 + var));
	hit_x = x(gen); hit_y = y(gen);									//이 좌표가 몬스터의 이미지 안쪽에 위치해야 대미지 판정을 받는다.
	angle = atan2(y(gen) - (CM_y + 60), x(gen) - (CM_x + 50));      //atan2 함수로 총알이 그려지는 각도를 계산한다.
	ammo_x1 = CM_x + 50; ammo_y1 = CM_y + 60;
	ammo_x2 = ammo_x1 + (1500 * cos(angle)); ammo_y2 = ammo_y1 + (1500 * sin(angle));
	 
	//히트 판정
	if(GUN_number != awp) check_hit();
		 
	//AWP일 경우 전용 히트 판정 함수 사용
	if (GUN_number == awp) check_hit_awp();
		 
	is_draw = TRUE; draw_hit = TRUE; //그리기 시작
	ind_effect = 1; shake_effect = 1; //각각 인터페이스 이펙트, 흔들림 이펙트
	shoot_delay = 0;	//딜레이는 0이되어 다시 딜레이가 증가하기 시작

	can_shoot = FALSE;
}

//사격
void shoot() {
	if (is_click == TRUE && reload == 0 && empty == 0) {
		if(shoot_delay < Gun::shoot_speed(GUN_number))
			shoot_delay++;
		if(shoot_delay == Gun::shoot_speed(GUN_number)) {
			make_rand_ammo(ammo, Gun::max_ammo(GUN_number)); //최대 장탄수를 오른쪽 인수에 적는다
			var += Gun::recoil(GUN_number); ammo++;
			is_zoom = FALSE; avail_awp = FALSE;
			ch_gun->stop(); ch_gun2->stop(); 
			switch(GUN_number) {
			case scar_h:
				ssystem->playSound(scar_shoot, 0, false, &ch_gun); //사운드 재생
				ssystem->playSound(scar_distance, 0, false, &ch_gun2); //사운드 재생
				break;
			case m16:
				ssystem->playSound(m16_shoot, 0, false, &ch_gun); //사운드 재생
				ssystem->playSound(m16_distance, 0, false, &ch_gun2); //사운드 재생
				break;
			case mp_44:
				ssystem->playSound(mp44_shoot, 0, false, &ch_gun); //사운드 재생
				ssystem->playSound(mp44_distance, 0, false, &ch_gun2); //사운드 재생
				break;
			case mg_42:
				ssystem->playSound(mg42_shoot, 0, false, &ch_gun);
				ssystem->playSound(mg42_distance, 0, false, &ch_gun2); //사운드 재생
				break;
			case awp:
				ssystem->playSound(awp_shoot, 0, false, &ch_gun);
				ssystem->playSound(awp_distance, 0, false, &ch_gun2); //사운드 재생 
				//볼트 작동 소리도 같이 출력
				ch_reload->stop(); ssystem->playSound(sniper_bolt, 0, false, &ch_reload);
				is_click = FALSE; cat_ready = 1;
				//탄피를 배출할 준비를 한다. 
				break;
			}

			if (GUN_number != awp) {
				if (CM_img_dir == 0) gc[cdx].x = CM_x + 10; 
				else if (CM_img_dir == 1) gc[cdx].x = CM_x + 90;
					 
				gc[cdx].y = CM_y + 50; gc[cdx].frame = 1;
				std::random_device rd_cat; std::mt19937 gen(rd_cat());
				 
				//탄피 랜덤 속도 생성
				std::uniform_int_distribution<int> cat_rand(6, 12); gc[cdx].acc = cat_rand(gen); 
				std::uniform_int_distribution<int> speed_rand(10, 15); gc[cdx].x_speed = speed_rand(gen);
				 
				gc[cdx].motion_dir = 1; //1: up 2: down
				if (CM_img_dir == 0) gc[cdx++].dir = 0; 
				else if (CM_img_dir == 1) gc[cdx++].dir = 1; 
			}
		}
		//is_hit 다시 초기화
		is_hit = FALSE;
	}
}

//awp 탄피 생성
void make_cat_awp() {
	if (CM_img_dir == 0) gc[cdx].x = CM_x + 10; 
	else if (CM_img_dir == 1) gc[cdx].x = CM_x + 90;
		 
	gc[cdx].y = CM_y + 50; gc[cdx].frame = 1;
	std::random_device rd_cat; std::mt19937 gen(rd_cat());
	 
	//탄피 랜덤 속도 생성
	std::uniform_int_distribution<int> cat_rand(6, 12); gc[cdx].acc = cat_rand(gen); 
	std::uniform_int_distribution<int> speed_rand(10, 15); gc[cdx].x_speed = speed_rand(gen);
	 
	gc[cdx].motion_dir = 1; //1: up 2: down
	if (CM_img_dir == 0) gc[cdx++].dir = 0; 
	else if (CM_img_dir == 1) gc[cdx++].dir = 1; 
}

//수류탄 폭발 판정
void check_explode_damage() {
	//범위 안에 있는 몬스터는 무조건 사망
	if (is_boom == TRUE) {
		int is_kill_r = 0; int is_kill_big = 0; 
		for (int i = mdx_r - 1; i >= 0; i--) {
			if (mst_r[i].x + 50 >= (gren_x - 90) - 400 && mst_r[i].x + 50 <= (gren_x - 90) + 400) mst_r[i].hp -= 150; 
			if (mst_r[i].hp <= 0) {
				if (i < mdx_r - 1 && is_kill_r == 0) {
					dl[ddx].x = mst_r[i].x; dl[ddx].y = mst_r[i].y; dl[ddx].monster_type = 1; dl[ddx].dir = mst_r[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					monster_array_push_r(i, mdx_r--); experience += 5; prev_up = 5; exp_up = TRUE;
					init_exp_animation(); is_kill_r = 1; kill_count++;
				}
				else if (i == mdx_r - 1 && is_kill_r == 0) {
					dl[ddx].x = mst_r[i].x; dl[ddx].y = mst_r[i].y; dl[ddx].monster_type = 1; dl[ddx].dir = mst_r[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					mdx_r--; experience += 5; prev_up = 5; exp_up = TRUE;
					init_exp_animation(); is_kill_r = 1; kill_count++;
				}
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp); is_kill_r = 0; 
			}
		}

		for (int i = mdx_big - 1; i >= 0; i--) {
			if (mst_big[i].x + 100 >= (gren_x - 90) - 400 && mst_big[i].x + 50 <= (gren_x - 90) + 400) mst_big[i].hp -= 150; 
			if (mst_big[i].hp <= 0) {
				if (i < mdx_big - 1 && is_kill_big == 0) {
					dl[ddx].x = mst_big[i].x; dl[ddx].y = mst_big[i].y; dl[ddx].monster_type = 2; dl[ddx].dir = mst_big[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					monster_array_push_big(i, mdx_big--); experience += 7; prev_up = 7; exp_up = TRUE;
					init_exp_animation(); is_kill_big = 1; kill_count++;
				}
				else if (i == mdx_big - 1 && is_kill_big == 0) {
					dl[ddx].x = mst_big[i].x; dl[ddx].y = mst_big[i].y; dl[ddx].monster_type = 2; dl[ddx].dir = mst_big[i].img_dir;
					dl[ddx].acc = 20; dl[ddx++].motion_dir = 1;
					mdx_big--; experience += 7; prev_up = 7; exp_up = TRUE;
					init_exp_animation(); is_kill_big = 1; kill_count++;
				}
				ch_exp->stop(); ssystem->playSound(exp_get, 0, false, &ch_exp); is_kill_big = 0;
			} 
		}
	}
}

//수류탄 생성
void make_grenade() {
	gren_dir = CM_img_dir; gren_motion_dir = 1;
	gren_x = CM_x + 50; gren_y = CM_y + 50;
	gren_acc = 10; g_frame = 0; gren_time = 30;
	ex_frame = 0; ex_frame_delay = 0;
	is_throw = TRUE; able_grenade = FALSE; boom_sound = FALSE; 
}

//수류탄 작동
void grenade_process() {
	//수류탄 대기시간이 1초에 1씩 줄어든다.
	if (able_grenade == FALSE) {
		if (gren_time == 0) able_grenade = TRUE;
		if (gren_delay < 65) gren_delay++;
		if (gren_delay == 65) {
			gren_time--; gren_delay = 0;
		}
	}

	//수류탄이 땅에 떨어지면 점화가 시작된다.
	if (set_grenade == TRUE && is_boom == FALSE) {
		if (boom_delay < 100) boom_delay++;
		if (boom_delay == 100) { //점화가 끝나면 폭발한다.
			is_boom = TRUE; boom_delay = 0;
		}
	}

	//수류탄 폭발음
	if (is_boom == TRUE && boom_sound == FALSE) {
		ch_explode->stop(); ssystem->playSound(ex_sound, 0, false, &ch_explode);
		boom_sound = TRUE; shake_effect = 3;
	}
}

//사격 관련 애니메이션 업데이트
void update_shoot_animation() {
	//awp 정조준
	if (GUN_number == awp) {
		//정조준 시 정확도가 점차 향상된다. 공중에 떠 있는 상태에서는 정조준이 되지 않는다.
		if (is_zoom == TRUE && CM_jump == 0) { 
			if (var > 0) var -= 6;
			if (var == 0) avail_awp = TRUE; //정조준을 완전히 해야 발사 가능
		}

		//정조준을 하지 않거나 정조준을 중단하면 정확도가 점차 떨어진다.
		if (is_zoom == FALSE) {
			if (var < 120)	var += 12;
		}

		//항상 정해진 넓이로 조준점이 유지되도록 한다.
		if (is_zoom == FALSE && var > 120) var -= 2;
	}

	//awp 정조준 완료 시 플레이어 머리 위로 사격 가능 표시가 올라온다
	if (avail_awp == TRUE && can_shoot == TRUE) {
		if (ypos_zc_acc > 0) ypos_zc += ypos_zc_acc--;
	}

	//awp 정조준을 해제하면 다시 아이콘이 내려간다
	else if (avail_awp == FALSE) {
		ypos_zc_acc = 15;
		if (ypos_zc > 0) ypos_zc -= ypos_zc_acc--;
	}

	//사격 시 화면 흔들림
	//좌측 값: 흔들리는 정도, 오른쪽 값: 흔들리는 시간
	if (shake_effect == 1) make_shake(Gun::shake(GUN_number), Gun::shake_time(GUN_number));

	if (shake_effect == 3) make_shake(30, 50);
	 
	//아주 짧은 시간동안 총알의 궤적을 그린다.
	if (is_draw == TRUE) { 
		draw_timer++;
		if (draw_timer == 3) {
			draw_timer = 0; is_draw = FALSE;
		}
	}

	//몬스터가 총알에 맞은 부분을 보여준다.
	if (draw_hit == TRUE) {
		hit_timer++;
		if (hit_timer == 10) {
			hit_timer = 0; draw_hit = FALSE;
		}
	}

	if (GUN_number != awp) {
		//사격 중지 시 조준점이 다시 회복됨
		if (is_click == FALSE || reload == 1 || empty == 1) {
			if (var >= 0) var -= 2;
		}

		//인터페이스 애니메이션 활성화
		if (ind_effect == 1) {
			ind_ani = 1; ind_size = 150; ind_x = 1335; ind_y = 625; ind_effect = 0; 
		}
	}

	//장탄수, 재장전 인디케이터 애니메이션
	if (ind_ani == 1) {
		ind_size -= 10; ind_x += 5; ind_y += 5; 
		if (ind_size == 100) ind_ani = 0;
	}

	//재장전 인디케이터
	if (reload == 1) { 
		if(reload_delay < Gun::reload_delay(GUN_number)) reload_delay++;
		if(reload_delay == Gun::reload_delay(GUN_number)) {
			reload_delay = 0; reload_x += Gun::reload_speed(GUN_number); 
			if(reload_x + CM_x >= CM_x + 100) {
				ammo = 0;  reload = 0;  r_pressed = 0; reload_x = 0; empty = 0;
				reload_delay = 0;
			}
		}
	}

	//경험치 증가 애니메이션
	if (exp_up == TRUE) {
		if (out == 1) {
			if(exp_acc > 0) exp_x += exp_acc--;
			if (exp_acc == 0) {
				exp_out_delay++;
				if (exp_out_delay == 20) {
					exp_out_delay = 0; out = 0; exp_acc = 20;
				}
			}
		}
		else if (out == 0) {
			if(exp_acc > 0) exp_x -= exp_acc--;
			if (exp_acc == 0) {
				exp_up = FALSE; out = 1; exp_acc = 20;
			}
		}
	}

	//탄피 회전 및 탄피 떨어지는 애니메이션
	for (int i = cdx - 1; i >= 0; i--) {
		//어느 방향에서 총을 쐈냐에 따라 탄피 회전 방향이 달라짐
		//motion_dir가 -1이 된 탄피는 더 이상 회전하지 않음
		if (gc[i].motion_dir != -1) {
			if (gc[i].dir == 0) {
				if (gc[i].frame < 3) gc[i].frame++; 
				if (gc[i].frame == 3) gc[i].frame = 0; 
			} 
			else if (gc[i].dir == 1) {
				if (gc[i].frame > 0) gc[i].frame --; 
				if (gc[i].frame == 0) gc[i].frame = 3; 
			}
		} 
		if (gc[i].dir == 0) { 
			//탄피가 총 밖으로 배출되며 살짝 올라간다
			if (gc[i].motion_dir == 1) {
				gc[i].x += gc[i].x_speed;
				if (gc[i].acc > 0) gc[i].y -= gc[i].acc--; 
				if (gc[i].acc == 0) gc[i].motion_dir = 2; 
			} 
			//탄피가 땅으로 떨어진다
			else if (gc[i].motion_dir == 2) {
				gc[i].x += gc[i].x_speed; gc[i].y += gc[i].acc++; 
				if (gc[i].y >= 700) {
					ch_cat->stop(); ssystem->playSound(cat_hit_ground, 0, false, &ch_cat); 
					gc[i].motion_dir = 3; gc[i].acc = 0; 
				}
			} 
			//탄피가 땅에 튕긴다
			else if (gc[i].motion_dir == 3) {
				gc[i].x += gc[i].x_speed - 2;
				if (gc[i].acc < 12) gc[i].y -= gc[i].acc++; 
				if (gc[i].acc == 12) {
					gc[i].motion_dir = 4; gc[i].acc = 0; 
				}
			} 
			//탄피는 더 이상 튕기지 않는다
			else if (gc[i].motion_dir == 4) {
				gc[i].x += gc[i].x_speed - 2; gc[i].y += gc[i].acc++; 
				if (gc[i].y >= 700) {
					ch_cat->stop(); ssystem->playSound(cat_stop, 0, false, &ch_cat); 
					gc[i].motion_dir = -1; gc[i].frame = 0; 
				}
			}
		} 
		else if (gc[i].dir == 1) {
			if (gc[i].motion_dir == 1) {
				gc[i].x -= gc[i].x_speed;
				if (gc[i].acc > 0) gc[i].y -= gc[i].acc--; 
				if (gc[i].acc == 0) gc[i].motion_dir = 2; 
			} 
			else if (gc[i].motion_dir == 2) {
				gc[i].x -= gc[i].x_speed; gc[i].y += gc[i].acc++; 
				if (gc[i].y >= 700) {
					ch_cat->stop(); ssystem->playSound(cat_hit_ground, 0, false, &ch_cat); 
					gc[i].motion_dir = 3; gc[i].acc = 0; 
				}
			} 
			else if (gc[i].motion_dir == 3) {
				gc[i].x -= gc[i].x_speed - 2;
				if (gc[i].acc < 12) gc[i].y -= gc[i].acc++; 
				if (gc[i].acc == 12) {
					gc[i].motion_dir = 4; gc[i].acc = 0; 
				}
			} 
			else if (gc[i].motion_dir == 4) {
				gc[i].x -= gc[i].x_speed - 2; gc[i].y += gc[i].acc++; 
				if (gc[i].y >= 700) {
					ch_cat->stop(); ssystem->playSound(cat_stop, 0, false, &ch_cat); 
					gc[i].motion_dir = -1; gc[i].frame = 0; 
				}
			}
		}
	}

	//수류탄 던지고 날아가는 애니메이션
	if (is_throw == TRUE) { //방향에 따라 수류탄이 회전하는 방향이 달라짐
		if (gren_dir == 1) {
			if (g_frame < 7) g_frame++; 
			if (g_frame == 7) g_frame = 0; 
		}
		else if (gren_dir == 0) { 
			if (g_frame > 0) g_frame--; 
			if (g_frame == 0) g_frame = 7; 
		} 
		//수류탄을 던져서 날아감
		if (gren_dir == 1) { 
			if (gren_motion_dir == 1) {
				gren_x += 15;
				if (gren_y > 0) gren_y -= gren_acc--;
				if (gren_acc == 0) gren_motion_dir = 2;
			}
			//떨어지기 시작
			else if (gren_motion_dir == 2) {
				gren_x += 15; gren_y += gren_acc++; 
				if (gren_y >= 650) { //땅에 닿으면 점화 시작 
					set_grenade = TRUE; is_throw = FALSE;
					gren_motion_dir = 0;
				}
			}
		}
		else if (gren_dir == 0) {
			if (gren_motion_dir == 1) {
				gren_x -= 15;
				if (gren_y > 0) gren_y -= gren_acc--;
				if (gren_acc == 0) gren_motion_dir = 2;
			}
			else if (gren_motion_dir == 2) {
				gren_x -= 15; gren_y += gren_acc++; 
				if (gren_y >= 650) { 
					set_grenade = TRUE; is_throw = FALSE;
					gren_motion_dir = 0;
				}
			}
		}
	} 
	//폭발 화염 애니매이션
	if (is_boom == TRUE) {
		check_explode_damage(); //몬스터 폭발 대미지 판정
		if (ex_frame_delay < 3) ex_frame_delay++; 
		if (ex_frame_delay == 3) {
			if (ex_frame < 6) ex_frame++; 
			if (ex_frame == 6) { 
				is_boom = FALSE; set_grenade = FALSE; 
			}
			ex_frame_delay = 0;
		}
	}

	//라운드 업 애니매이션
	if (round_up == TRUE) {
		if (round_size > 70) {
			round_size -= 5;
			round_x += 7;
		}
		if (round_size == 70) round_up = FALSE;
	}


	//awp의 경우 볼트를 당겨야 탄피가 배출된다.
	if (cat_ready == 1) {
		if (awp_cat_delay < 55)awp_cat_delay++;
		if (awp_cat_delay == 55) {
			awp_cat_delay = 0; cat_ready = 0; make_cat_awp();
		}
	}
}

//몬스터 공격 판정
void check_monster_attack() {
	//일반 몬스터 공격 판정
	for (int i = mdx_r - 1; i >= 0; i --) {
		if (cal_dist(CM_x + 50, CM_y + 50, mst_r[i].x + 50, mst_r[i].y + 50) <= 50 &&  cool_time == 0) {
			health -= 10; mst_r[i].motion_acc = 10;
			mst_r[i].height = 155; mst_r[i].y = 545;
			cool_time = 30;
				
			ch_hurt->stop(); ssystem->playSound(hurt, 0, false, &ch_hurt);
			ch_mst_attack_sound->stop();  play_attack_sound();
			shake_effect = 2;
		}
	} 
	//대형 몬스터 공격 판정
	for (int i = mdx_big - 1; i >= 0; i--) {
		if (cal_dist(CM_x + 50 , CM_y + 50, mst_big[i].x + 100, mst_big[i].y + 100) <= 100 && cool_time == 0) {
			health -= 15;
			cool_time = 30;  mst_big[i].height = 255;
			mst_big[i].y = 445;	mst_big[i].motion_acc = 10;
				
			ch_hurt->stop(); ssystem->playSound(hurt, 0, false, &ch_hurt);
			ch_mst_attack_sound->stop();  play_attack_sound();
			shake_effect = 2;
		}
	} 
	//공중 몬스터 공격 판정
	for (int i = mdx_air - 1; i >= 0; i--) {
		if (cal_dist(CM_x + 50, CM_y + 50, mst_air[i].x + 75, mst_air[i].y + 30) <= 75 && cool_time == 0) {
			health -= 5;
			cool_time = 30; mst_air[i].height = 115;
			mst_air[i].y = 145; mst_air[i].motion_acc = 10;
				
			ch_hurt->stop(); ssystem->playSound(hurt, 0, false, &ch_hurt);
			shake_effect = 2;
			
		} 
	}
}

//인덱스 삭제
void index_auto_delete() {
	//시체 인덱스 삭제
	if (ddx > 0) {
		if (delete_delay < 70) delete_delay++;
		if (delete_delay == 70) {
			push_dead(ddx--); delete_delay = 0;
		}
	}
	//탄피 인덱스 삭제 
	if (cdx > 5) {  //mg42는 연사 속도가 빨라 다른 총들보다 더 빨리 삭제함
		if (GUN_number == mg_42) {
			if (cat_delete_delay < 5) cat_delete_delay++;
			if (cat_delete_delay == 5) {
				cat_delete_delay = 0;  push_cat(cdx--);
			}
		}
		if (GUN_number != mg_42) {
			if (cat_delete_delay < 8) cat_delete_delay++;
			if (cat_delete_delay == 8) {
				cat_delete_delay = 0; push_cat(cdx--);
			}
		}
	}
}

//플레이어 사운드
void play_player_sound() {
	// 점프 사운드
	if (jumping && space_pressed == 0) {
		if (space_pressed == 0 && is_zoom == FALSE) {
			CM_jump = 1; space_pressed = 1; is_zoom = FALSE;
			ch_jump->stop(); ssystem->playSound(jump, 0, false, &ch_jump); //사운드 재생 
		}
	}
	//걷는 소리
	if ((CM_move_dir == 1 || CM_move_dir == 0) && CM_jump == 0) {
		if (walk_sound_delay < 25) walk_sound_delay++;
		if (walk_sound_delay == 25) {
			walk_sound_delay = 0;
			ch_walk->stop(); ssystem->playSound(walk, 0, false, &ch_walk); //사운드 재생 
		}
	}
	//플레이어가 죽을 경우 죽는 소리가 재생
	if (health <= 0) {
		ch_dead->stop(); ssystem->playSound(dead, 0, false, &ch_dead);
	}
}

//몬스터 애니메이션
void monster_animation() {
	//공중 몬스터 idle 애니메이션
	if (up_down == 1) {
		Fdelay_air++;
		if (Fdelay_air == 7) {
			air++; Fdelay_air = 0;
			if (air == 2)  up_down = 0;
		}
	}
	else if (up_down == 0) {
		Fdelay_air++;
		if (Fdelay_air == 7) {
			air--; Fdelay_air = 0;
			if (air == 0) up_down = 1;
		}
	}

	//몬스터 공격 애니메이션
	for (int i = mdx_r - 1; i >= 0; i--) {
		if (mst_r[i].y < 600) {
			mst_r[i].y += mst_r[i].motion_acc; mst_r[i].height -= mst_r[i].motion_acc--;
		}
	}
	for (int i = mdx_big - 1; i >= 0; i--) {
		if (mst_big[i].y < 500) {
			mst_big[i].y += mst_big[i].motion_acc; mst_big[i].height -= mst_big[i].motion_acc--;
		}
	}
	for (int i = mdx_air - 1; i >= 0; i--) {
		if (mst_air[i].y < 200) {
			mst_air[i].y += mst_air[i].motion_acc; mst_air[i].height -= mst_air[i].motion_acc--;
		}
	}

	//몬스터에게 대미지를 받으면 화면이 흔들린다.
	if (shake_effect == 2) make_shake(15, 10);

	for (int i = ddx - 1; i >= 0; i--) {
		//공중 몬스터 시체는 하늘에서 떨어진다.
		if (dl[i].monster_type == 3 && dl[i].acc < 30) dl[i].y += dl[i].acc++;

		//지상 몬스터 사망 애니메이션
		if (dl[i].monster_type == 1 || dl[i].monster_type == 2) {
			if (dl[i].motion_dir == 1) {
				if (dl[i].acc > -1) dl[i].y -= dl[i].acc--;
				if (dl[i].acc == -1) dl[i].motion_dir = 2;
			}
			if (dl[i].motion_dir == 2) {
				if (dl[i].acc < 21)	dl[i].y += dl[i].acc++;
				if (dl[i].acc == 21) dl[i].motion_dir = -1;
			}
		}
	}
}





//UI 애니메이션
void UI_animation() {
	//일시정지 화면 애니메이션
	if (manager.isPaused()) {
		if (pause_acc > 0) {
			pause_y -= pause_acc; pause_acc -= 4;
			if(pause_y < 0) {
				pause_y = 0;
			}
		}
		if (pause_acc == 0) {
			if (cm_pause_acc > 0) {
				CM_paused_y -= cm_pause_acc; cm_pause_acc -= 4;
			}
		}
	}
	if (!manager.isPaused() && is_resumed == TRUE) {
		if (pause_acc > 0) {
			pause_y += pause_acc--; pause_acc -= 4;
		}
		if (cm_pause_acc > 0) {
			CM_paused_y += cm_pause_acc--; cm_pause_acc -= 4;
		}
		if (pause_acc == 0) is_resumed = FALSE;
	}
	//게임 오버 화면 애니메이션
	if (manager.isGameOver()) {
		if (death_acc > 0) death_x += death_acc--;
	}
	//메인화면 및 아머리씬 배경 스크롤 애니메이션
	if (manager.getCurrentSceneID() == Main || manager.getCurrentSceneID() == Armory || (manager.getCurrentSceneID() == Game && into_the_game == TRUE)) {
		if (Scanner_main < 1500) Scanner_main += 5; 
		if (Scanner_main == 1500) Scanner_main = 0; 
	}

	//로고 애니메이션
	//메인->아머리씬으로 이동 시
	if (manager.getCurrentSceneID() == Armory && main_to_armory == TRUE) {
		if (logo_acc > 0) logo_y -= logo_acc--; 
		if (logo_acc == 0) main_to_armory = FALSE;  
	}
	//아머리씬->메인으로 이동 시
	if (manager.getCurrentSceneID() == Main && armory_to_main == TRUE) {
		if (logo_acc > 0) logo_y += logo_acc--; 
		if (logo_acc == 0) armory_to_main = FALSE; 
	}
	//새 게임 시작 시
	if (manager.getCurrentSceneID() == Game && into_the_game == TRUE) {
		if (logo_acc > 0) logo_y -= logo_acc--; 
	}
}

//intro 애니메이션
void intro_animation() {
	//인트로 음악
	if (intro_sound == FALSE) {
		ch_bgm->stop(); ssystem->playSound(intro, 0, false, &ch_bgm); 
		intro_sound = TRUE;
	} 
	//인트로 애니메이션
	if (intro_time > 0) intro_time--;
	if (intro_time == 0) {
		is_intro = FALSE;
		logo_acc = 30; //로고 애니메이션 값 초기화
		logo_y = -415;
		armory_to_main = TRUE;
	}
	if (intro_time <= 500 && intro_time >= 410) ellipse_size++;
	if (intro_time < 410 && intro_time >= 210) ellipse_size += 10;
	if (intro_time < 300) {
		if (intro_logo_acc > 0) intro_logo_y -= intro_logo_acc--; 
	}
	if (intro_logo_acc == 0 && intro_time > 155) ellipse2_size += 12;
	//배경이 점차 어두워진다
	if (intro_time < 150) {
		if (r > 0) {
			r -= 2.5; g -= 2.5; b -= 2.5; 
		}
	}
}

//새 게임 애니메이션
void new_game_animation(RECT rt) {
	if (new_game_sound == FALSE) {
		ch_bgm->stop(); ssystem->playSound(new_game, 0, false, &ch_bgm);
		new_game_sound = TRUE;	logo_acc = 30; logo_y = 50; //로고 위치 초기화
	}
	if(new_acc > 0) {
		new_bg_y -= new_acc--;
		if(new_bg_y < 0) {
			new_bg_y = 0;
		}
	}
	if (new_acc == 0) {
		if (new_logo_move == 1) {
			if (new_logo_acc > 0) new_logo_y -= new_logo_acc--;
			if (new_logo_acc == 0) {
				if (new_logo_delay < 100) new_logo_delay++;
				if (new_logo_delay == 100) {
					new_logo_acc = 35; new_logo_move = 2;
				}
			}
		}
		else if (new_logo_move == 2) {
			if(new_logo_acc > 0) new_logo_y -= new_logo_acc--;
		}
	}
	if (into_time > 0) into_time--; 
	if (into_time == 0) {								//이 변수가 TRUE가 되면 새 게임 애니메이션을 끝내고 게임 업데이트 및 출력을 시작한다.
		new_game_sound = FALSE; into_the_game = FALSE; end_new_game = TRUE;
		into_time = 200; new_acc = 41;
	}
}

//LBUTTONDOWN
void wm_lbuttondown() {
	//awp가 아닌 다른 총은 즉시 발사 가능
	if(GUN_number != awp) is_click = TRUE;

	//awp 정조준 안할 시 총이 나가지 않고 빈 총 소리만 남
	if (GUN_number == awp && ((avail_awp == FALSE && is_zoom == TRUE) || (avail_awp == FALSE && is_zoom == FALSE))) {
		ch_dry->stop(); ssystem->playSound(dry_fire, 0, false, &ch_dry); 
	}

	//awp는 정조준이 완료되어야 발사 가능
	if (GUN_number == awp && avail_awp == TRUE)	is_click = TRUE;

	if (empty == 1 && reload == 0) {
		reload = 1;
		if (GUN_number == scar_h || GUN_number == m16 || GUN_number == mp_44) {
			ch_reload->stop(); ssystem->playSound(rifle_reload, 0, false, &ch_reload); //사운드 재생 
		}
		if (GUN_number == mg_42) {
			ch_reload->stop(); ssystem->playSound(lmg_reload, 0, false, &ch_reload); //사운드 재생 
		}
		if (GUN_number == awp) {
			ch_reload->stop(); ssystem->playSound(sniper_reload, 0, false, &ch_reload); 
		}
	}

	//연사력이 높을수록 딜레이 수치는 낮음
	if(GUN_number != awp) shoot_delay = Gun::shoot_speed(GUN_number); 
	if(GUN_number == awp && avail_awp == TRUE) shoot_delay = Gun::shoot_speed(GUN_number); 
}

//WM_RBUTTONDOWN
void wm_rbuttondown() {
	if (reload == 0 && r_pressed == 0 && empty == 0) {
		is_zoom = TRUE; CM_move_dir = -1;
		ch_zoom->stop(); ssystem->playSound(zoom_sound, 0, false, &ch_zoom); 
	}
}

//그리기 파트
void wm_paint(HDC mdc, RECT rt) {
	//////////////////////// 버퍼
	BG_w = 1500; BG_h = BackGround.GetHeight();
	BackGround.Draw(mdc, rt.left + ss_x, rt.top - 30 + landing_shake + ss_y, rt.right, rt.bottom + 30, BG_scanner, 0, BG_w, BG_h);
	//BG_scanner가 클수록 배경은 오른쪽으로 이동하게 됨

	//정조준 완료 시에 관통 대상 표시
	if (GUN_number == awp && is_zoom == TRUE && empty == 0 && reload == 0 && r_pressed == 0) {
		find_target(mx, my); show_awp_targeted(mdc); 
	}

	//awp 정조준 완료 표시
	show_zoom_complited(mdc);

	//몬스터 이미지 출력
	show_monster(mdc, ss_x, ss_y, landing_shake);

	//총알 궤적 그리기
	if (is_draw == TRUE) draw_ammo(mdc, ammo_x1, ammo_y1, ammo_x2, ammo_y2, GUN_number);

	//플레이어 이미지 출력
	show_player(mdc);

	//탄피 이미지 출력
	show_catridge(mdc, ss_x, ss_y, landing_shake);

	//수류탄 이미지 출력
	if(is_throw == TRUE || set_grenade == TRUE) show_grenade(mdc, ss_x, ss_y, landing_shake);
		 
	//히트 포인트 그리기
	if (draw_hit == TRUE) show_hit(mdc, ammo_x2, ammo_y2);

	//인터페이스 출력
	show_interface(mdc, rt);

	//조준점 출력
	if (!manager.isPaused() && !manager.isGameOver()) show_target(mdc, mx + ss_x, my + ss_y + landing_shake, var);

	//게임 오버 씬
	if (manager.isGameOver()) {
		HBRUSH hbrush, oldbrush;
		hbrush = CreateSolidBrush(RGB(0, 0, 0));
		oldbrush = (HBRUSH)SelectObject(mdc, hbrush); 
		Rectangle(mdc, rt.left, rt.top, rt.right, rt.bottom); 
		SelectObject(mdc, oldbrush); DeleteObject(hbrush); 
		CM_dead.Draw(mdc, death_x, 200, 500, 500, 0, 0, 500, 500);
	} 
}

void syncSize(const RECT& rect) {
	// 플레이어 이미지 사이즈 맞추기
	// 플레이어 위치 맞추기
	// 배경 비율 유지하면서 출력
	// 총 이미지 사이즈 맞추기
	// 총 위치 맞추기
	// 몬스터 이미지 사이즈 맞추기
	// 몬스터 위치 맞추기
	// 각종 애니메이션에 쓰이는 스프라이트 사이즈 맞추기
	// 충돌체크할때 사이즈도 맞춰야됨
	// 수류탄 이미지 사이즈 맞추기
	// 탄피 이미지 사이즈 맞추기
	// 조준점 크기
	// 탄퍼짐 정도 사이즈 상관 없이 일정하게 조정
	
	// 스크린 크기에 의존한 값이 너무 많다
	// -> 이걸 기본 좌표계로 설정하고 화면 크기 변경 정도를 구해서 적용하도록..
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, mdc;  PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt;
	GetClientRect(hWnd, &rt);
	syncSize(rt);

	switch(uMsg) {
	case WM_CREATE:
		set_FMOD(); IMG_FILE_LOAD();  //초기 세팅
		CM_x = 700, CM_y = 600;           //초기 플레이어 위치
		SetTimer(hWnd, UPDATE, 5, NULL);  //게임 전체 타이머, 추후 애니메이션 전용 타이머도 추가 예정
		is_intro = TRUE;				  //비활성화 시 인트로를 건너 뛰고 게임으로 바로 넘어감
		break;

	case WM_ACTIVATE:
		switch(LOWORD(wParam)) {
		case WA_INACTIVE:
			manager.interrupt(); break;
		}
		break;

	case WM_KEYDOWN:
		if(manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver() && into_the_game == FALSE) {
			switch(wParam) {
			case L'A': case L'a':
				if(is_zoom == FALSE) {
					CM_move_dir = 0; left_pressed = true; 
				} break;
				 
			case L'D': case L'd':
				if(is_zoom == FALSE) {
					CM_move_dir = 1; right_pressed = true; 
				} break;
				 
			case L'R': case L'r':
				if(r_pressed == 0 && reload == 0) {
					reload = 1; r_pressed = 1;
					if(GUN_number == scar_h || GUN_number == m16 || GUN_number == mp_44) {
						ch_reload->stop(); ssystem->playSound(rifle_reload, 0, false, &ch_reload); //사운드 재생 
					}
					if(GUN_number == mg_42) {
						ch_reload->stop(); ssystem->playSound(lmg_reload, 0, false, &ch_reload); //사운드 재생 
					}
					if(GUN_number == awp) {
						ch_reload->stop(); ssystem->playSound(sniper_reload, 0, false, &ch_reload);
						is_zoom = FALSE; avail_awp = FALSE; 
					}
				} break;
				 
			case VK_SHIFT: //수류탄 던지기
				if (able_grenade == TRUE) {
					make_grenade(); shift_pressed = true;
					ch_explode->stop(); ssystem->playSound(pin_sound, 0, false, &ch_explode); 
				} break;
			 
			case VK_SPACE:
				jumping = true;
				break;
			}
		}
		if(into_the_game == FALSE) manager.keyboardInput(hWnd, wParam); 
		break;

		//정지상태로 변경
	case WM_KEYUP:
		switch(wParam) {
		case L'A': case L'a':
			if(right_pressed) CM_move_dir = 1;
			else CM_move_dir = -1;
			left_pressed = false;
			break;

		case L'D': case L'd':
			if(left_pressed) CM_move_dir = 0; 
			else CM_move_dir = -1; 
			right_pressed = false;
			break;

		case VK_SPACE:
			jumping = false;
			break;

		case VK_SHIFT:
			shift_pressed = false;
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		if (manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver() && can_shoot == TRUE) wm_lbuttondown();
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Left);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
		break;

	case WM_RBUTTONDOWN:
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Right);
		if(manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver()) {
			if(GUN_number == awp) wm_rbuttondown();
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_RBUTTONUP:
		is_zoom = FALSE; avail_awp = FALSE; 
		if(manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver()) {
			if(GUN_number == awp && reload == 0 && r_pressed == 0 && empty == 0) {
				ch_zoom->stop(); ssystem->playSound(unzoom_sound, 0, false, &ch_zoom);
				for(int i = 0; i < mdx_r; i++) mst_r[i].targeted = 0;
				for(int i = 0; i < mdx_big; i++) mst_big[i].targeted = 0;
			}
		}
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam); my = HIWORD(lParam);
		if(manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver())  update_player_direction(mx);
		break;

	case WM_TIMER:
		switch (wParam) {
		case UPDATE: //게임 전체 타이머
			GetClientRect(hWnd, &rt);

			//인트로 중에는 인트로 외에는 어떠한 다른 작업도 실행되지 않는다.
			if (is_intro == TRUE) {
				if (intro_delay > 0) intro_delay--;
				if (intro_delay == 0) intro_animation();
			}

			//새 게임 시작 시 애니메이션을 재생한다.
			if (into_the_game == TRUE) new_game_animation(rt);
			if (into_the_game == FALSE && end_new_game == TRUE) {
				CM_game_start_x += 100;
				if (CM_game_start_x > rt.left - 500) {
					new_bg_x += 100;
					if (whoosh_sound == FALSE) {
						ch_round->stop();  ssystem->playSound(woosh, 0, false, &ch_round);
						whoosh_sound = TRUE;
					}
				}
				if (CM_game_start_x > rt.right) {
					end_new_game = FALSE; whoosh_sound = FALSE;
				}
			}

			//인트로가 끝나면 버튼 사운드 출력과 UI애니메이션을 출력한다.
			if (is_intro == FALSE) {
				play_button_sound(); UI_animation(); 
			}

			//인트로와 새 게임 애니메이션이 재생되지 않아야 게임을 업데이트 한다.
			if (is_intro == FALSE && into_the_game == FALSE) {
				 manager.update(hWnd);
				
				//메인 화면 브금
				if (manager.getCurrentSceneID() == Main && main_bgm_on == FALSE) {
					ch_bgm->stop(); ssystem->playSound(main_bgm, 0, false, &ch_bgm);
					main_bgm_on = TRUE; gameover_bgm_on = FALSE; game_bgm_on = FALSE;
				}
				//인 게임 브금
				if (manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver() && game_bgm_on == FALSE) {
					ch_bgm->stop(); ssystem->playSound(game_bgm, 0, false, &ch_bgm);
					main_bgm_on = FALSE; pause_bgm_on = FALSE; game_bgm_on = TRUE;
				}
				//게임 오버 브금
				if (manager.isGameOver() && gameover_bgm_on == FALSE) {
					ch_bgm->stop(); ssystem->playSound(gameover_bgm, 0, false, &ch_bgm);
					gameover_bgm_on = TRUE; game_bgm_on = FALSE;
				}
				//일시 정지 브금
				if (manager.isPaused() && pause_bgm_on == FALSE) {
					ch_bgm->stop(); ssystem->playSound(pause_bgm, 0, false, &ch_bgm);
					game_bgm_on = FALSE; pause_bgm_on = TRUE;
				}
				
				//인 게임
				if (manager.getCurrentSceneID() == Game && !manager.isPaused() && !manager.isGameOver()) {
					update_monster_direction(CM_x); update_player_position(rt);
					update_monster_position();      update_shoot_animation();
					check_monster_attack();         monster_animation();
					make_monster(rt);               shoot();
					index_auto_delete();			grenade_process();
					play_idle_sound();				play_player_sound();

					//연사 속도가 느린 총을 마우스 광클로 빨리 쏘는 꼼수 방지
					if (can_shoot == FALSE) mouse_fastClick_prevention();

					//체력이 100보다 낮을 경우 자가 회복
					if (health < 100) {
						if (recovery_delay < 100) recovery_delay++;
						if (recovery_delay == 100) {
							health++; recovery_delay = 0;
						}
					}
					//라운드 업 사운드
					if (round_up_sound == TRUE) {
						ch_round->stop();
						ssystem->playSound(next_round, 0, false, &ch_round);
						round_up_sound = FALSE;
					}
					//대미지를 연속으로 입지 않도록 쿨타임을 조성한다.
					if (cool_time > 0) cool_time--;
				}
			}

			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
			GetClientRect(hWnd, &rt);
			hdc = BeginPaint(hWnd, &ps);
			mdc = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			(HBITMAP)SelectObject(mdc, hbitmap); 

			syncSize(rt);

			////인트로에 나오는 원 애니메이션 파트
			//if (is_intro == TRUE) {
			//	ellipse_intro(mdc, rt, ellipse_size, r, g, b);
			//	if(intro_logo_acc == 0 && intro_time > 155)	ellipse_intro2(mdc, rt, ellipse2_size);
			//	//intro_logo.Draw(mdc, intro_logo_x, intro_logo_y, 700, 300, 0, 0, 700, 300);
			//	Sprite intro_logo { L"./res/intro_logo.png" };
			//	intro_logo.fix_ratio = true;
			//	//intro_logo.position.x = intro_logo_x / 700.0 * rt.right;
			//	intro_logo.position.y = intro_logo_y / 850.0 * rt.bottom;
			//	RECT r = percentOf(rt, 50, Up);
			//	intro_logo.draw(mdc, r);
			//}
			is_intro = FALSE;
			into_the_game = FALSE;

			//메인 스크롤 백그라운드
			if (is_intro == FALSE) {
				if (manager.getCurrentSceneID() == Main || manager.getCurrentSceneID() == Armory 
					|| (manager.getCurrentSceneID() == Game && into_the_game == TRUE)) {
					background_main.Draw(mdc, rt.left, rt.top, rt.right, rt.bottom, Scanner_main, 0, 1500, 800);
					//logo.Draw(mdc, rt.right/2-300, logo_y, 600, 300, 0, 0, 600, 300);
					Sprite game_logo { L"./res/logo.png" };
					game_logo.fix_ratio = true;
					game_logo.position.y = logo_y / 300.0 * rt.bottom/3;
					game_logo.draw(mdc, { rt.left, rt.top, rt.right, rt.bottom/3 });
				}
			}

			//인트로와 새 게임 애니메이션이 재생되지 않아야 게임 화면을 출력한다.
			if (is_intro == FALSE && into_the_game == FALSE) {
				if (manager.getCurrentSceneID() == Game) wm_paint(mdc, rt);

				//일시정지 씬
				if (manager.isPaused() || is_resumed == TRUE) {
					BG_paused.Draw(mdc, rt.left, rt.top + pause_y / 800.0 * rt.bottom, rt.right, rt.bottom, 0, 0, 1500, 800);
					//CM_paused.Draw(mdc, rt.right - 550, CM_paused_y, 550, 800, 0, 0, 550, 800);
					Sprite sp { L"./res/commando_paused.png" };
					sp.fix_ratio = true;
					sp.position.y = CM_paused_y / 800.0 * rt.bottom;
					RECT r = convertRatio(rt, 550, 800, Right);
					sp.draw(mdc, r);
				}

				manager.syncSize(hWnd); manager.show(mdc);
			}


			// 게임 시작시 애니메이션
			if (into_the_game == TRUE || end_new_game == TRUE) {
				background_game_start.Draw(mdc, new_bg_x / 800.0 * rt.right, new_bg_y / 850.0 * rt.bottom, rt.right, rt.bottom, 0, 0, 1500, 800);
				//CM_game_start.Draw(mdc, CM_game_start_x, rt.top, 800, 800, 0, 0, 800, 800);
				Sprite sp { L"./res/commando_game_start.png" };
				sp.fix_ratio = true;
				sp.position.x = CM_game_start_x / 800.0 * rt.right;
				sp.draw(mdc, rt);
				//logo_game_start.Draw(mdc, 400, new_logo_y, 700, 300, 0, 0, 700, 300);
				Sprite logo { L"./res/game_start_logo.png" };
				logo.fix_ratio = true;
				logo.position.y = new_logo_y / 850.0 * rt.bottom;
				RECT r = percentOf(rt, 50, Up);
				logo.draw(mdc, r);
			}
			
			BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
		
			DeleteDC(mdc); DeleteObject(hbitmap); EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		DestroyWindow(hWnd); PostQuitMessage(0);
		ssystem->close(); ssystem->release();
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	WNDCLASSEX WndClass;
	MSG Message;

	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 50, 1500, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}