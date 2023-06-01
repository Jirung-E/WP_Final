#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include <math.h>
#include <random>
#include <iostream>
#include "GameManager.h"
#include "monster_info.h"//몬스터 정보 헤더
#include "ammo.h"        //총알 정보 헤더
#include "player_info.h" //플레이어 정보 헤더
#include "images.h"      //이미지 정보 헤더
#include "exp.h"         //경험치 정보 헤더, 경험치로 총기를 구입하고, 소비한다.

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

std::random_device rd_main;
std::mt19937 gen(rd_main());

GameManager manager;
extern monster_info_regular mst_r[100]; //몬스터 정보를 저정하는 구조체, monster_info.h에 선언
extern monster_info_big mst_big[100];
extern monster_info_air mst_air[100];

enum Timer {
	KEYDOWN, UPDATE
};

//마우스 좌표
static double mx, my;
//마우스 클릭 여부
static BOOL is_click = FALSE; 

//이미지 파일 로드
void IMG_FILE_LOAD() {
	BackGround.Load(L".\\res\\BackGround_wide.png");

	commando_right.Load(L".\\res\\commando_right.png");
	commando_left.Load(L".\\res\\commando_left.png");
	commando_jump_right.Load(L".\\res\\commando_jump_right.png");
	commando_jump_left.Load(L".\\res\\commando_jump_left.png");

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
} 

//조준점 출력
void show_target(HDC mdc, int mouse_x, int mouse_y, double var) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, mouse_x + 10 + var, mouse_y, NULL);
	LineTo(mdc, mouse_x + 40 + var, mouse_y);
	MoveToEx(mdc, mouse_x - 10 - var, mouse_y, NULL);
	LineTo(mdc, mouse_x - 40 - var, mouse_y);
	MoveToEx(mdc, mouse_x, mouse_y - 10 - var, NULL);
	LineTo(mdc, mouse_x, mouse_y - 40 - var);
	MoveToEx(mdc, mouse_x, mouse_y + 10 + var, NULL);
	LineTo(mdc, mouse_x, mouse_y + 40 + var);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
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

//인터페이스 출력
void show_interface(HDC mdc, RECT rt) {
	//장탄수 표시기 배경
	IND_w = indicator_back.GetWidth(); IND_h = indicator_back.GetHeight();
	indicator_back.Draw(mdc, rt.right - 600 + ss_x, rt.bottom - 110 + landing_shake + ss_y, 600, 110, 0, 0, IND_w, IND_h);

	//총알 아이콘
	if (GUN_number == scar_h || GUN_number == m16 || GUN_number == mp_44) {
		AMO_w = ammo_icon.GetWidth(); AMO_h = ammo_icon.GetHeight();
		ammo_icon.Draw(mdc, rt.right - 260 + ss_x, rt.bottom - 108 + landing_shake + ss_y, 100, 100, 0, 0, AMO_w, AMO_h);
	}

	//총 아이콘 및 장탄수 출력
	switch (GUN_number) {
	case scar_h:
		GUN_w = SCAR_H_right.GetWidth(); GUN_h = SCAR_H_right.GetWidth();
		SCAR_H_right.Draw(mdc, rt.right - 430 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 150, 150, 0, 0, GUN_w, GUN_h);
		//mdc 오른쪽에 최대 장탄수, 그 오른쪽에 현재 장탄수 입력
		ammo_indicator(mdc, 30, ammo, ind_size, ind_x + ss_x, ind_y + landing_shake + ss_y);
		break;
	}
	//경험치 수치 출력
	show_exp(mdc, experience, rt.left + 150 + ss_x, rt.top + 80 + ss_y + landing_shake);
	exp_icon.Draw(mdc, rt.left + 20 + ss_x, rt.top + 110 + ss_y + landing_shake, 100, 50, 0, 0, 100, 50);
	show_exp_add(mdc, prev_up, exp_x + ss_x, rt.top + 170 + ss_y + landing_shake);

	//재장전 게이지 출력
	if (reload == 1)
		reload_indicator(mdc, CM_x, CM_y - 30 + landing_shake, CM_x + reload_x, CM_y - 10 + landing_shake, CM_x, CM_y - 30 + landing_shake, CM_x + 100, CM_y - 10 + landing_shake);

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
}

//플레이어 이미지, 총 이미지 출력
void show_player(HDC mdc) {
	switch (CM_img_dir) { //플레이어, 총 이미지 출력
	case 0:
		if (CM_jump == 0) {
			CM_w = commando_left.GetWidth(); CM_h = commando_left.GetHeight();
			commando_left.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			CM_w = commando_jump_left.GetWidth(); CM_h = commando_jump_left.GetHeight();
			commando_jump_left.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
		}

		switch (get_GUN_number()) {
		case scar_h:
			GUN_w = SCAR_H_left.GetWidth(); GUN_h = SCAR_H_left.GetWidth();
			SCAR_H_left.Draw(mdc, CM_x - 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break;
		}

		break;
		//////////////////////
	case 1:
		if (CM_jump == 0) {
			CM_w = commando_right.GetWidth(); CM_h = commando_right.GetHeight();
			commando_right.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			CM_w = commando_jump_right.GetWidth(); CM_h = commando_jump_right.GetHeight();
			commando_jump_right.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
		}

		switch (get_GUN_number()) {
		case scar_h:
			GUN_w = SCAR_H_right.GetWidth(); GUN_h = SCAR_H_right.GetWidth();
			SCAR_H_right.Draw(mdc, CM_x + 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h);
			break;
		}

		break;
	}
}

//몬스터 생성
void make_monster(RECT rt) {
	//spawn_timer의 수치가 0이 되면 몬스터가 한 마리씩 스폰
	//일반 몬스터
	spawn_timer_r--;
	if (spawn_timer_r == 0) {
		spawn_timer_r = 500;
		if (mdx_r < 99) {
			spawn_monster_regular(mdx_r, BG_scanner, rt); mdx_r++;
		}
	}

	//대형 몬스터
	spawn_timer_big--;
	if (spawn_timer_big == 0) {
		spawn_timer_big = 1000;
		if (mdx_big < 99) {
			spawn_monster_big(mdx_big, BG_scanner, rt); mdx_big++;
		}
	}

	//공중 몬스터
	spawn_timer_air--;
	if (spawn_timer_air == 0) {
		spawn_timer_air = 600;
		if (mdx_air < 99) {
			spawn_monster_air(mdx_air, BG_scanner, rt); mdx_air++;
		}
	}
}

//몬스터 이미지 출력
void show_monster(HDC mdc, int ss_x, int ss_y, int landing_shake) {
	//일반 몬스터 출력
	for (int i = 0; i < mdx_r; i++) {
		switch (mst_r[i].img_dir) {
		case 0:
			MST_w = monster_left.GetWidth();
			MST_h = monster_left.GetHeight();
			monster_left.Draw(mdc, mst_r[i].x + ss_x, mst_r[i].y + ss_y + landing_shake, 100, 100, 0, 0, MST_w, MST_h);
			break;

		case 1:
			MST_w = monster_right.GetWidth();
			MST_h = monster_right.GetHeight();
			monster_right.Draw(mdc, mst_r[i].x + ss_x, mst_r[i].y + ss_y + landing_shake, 100, 100, 0, 0, MST_w, MST_h);
			break;
		}
	}

	//대형 몬스터 출력
	for (int i = 0; i < mdx_big; i++) {
		switch (mst_big[i].img_dir) {
		case 0:
			MST_big_w = monster_big_left.GetWidth();
			MST_big_h = monster_big_left.GetHeight();
			monster_big_left.Draw(mdc, mst_big[i].x + ss_x, mst_big[i].y + ss_y + landing_shake, 200, 200, 0, 0, MST_big_w, MST_big_h);
			break;

		case 1:
			MST_big_w = monster_big_right.GetWidth();
			MST_big_h = monster_big_right.GetHeight();
			monster_big_right.Draw(mdc, mst_big[i].x + ss_x, mst_big[i].y + ss_y + landing_shake, 200, 200, 0, 0, MST_big_w, MST_big_h);
			break;
		}
	}

	//공중 몬스터 출력
	for (int i = 0; i < mdx_air; i++) {
		switch (mst_air[i].img_dir) {
		case 0:
			MST_air_w = monster_air_left[air].GetWidth();
			MST_air_h = monster_air_left[air].GetHeight();
			monster_air_left[air].Draw(mdc, mst_air[i].x + ss_x, mst_air[i].y + ss_y + landing_shake, 150, 60, 0, 0, MST_air_w, MST_air_h);
			break;

		case 1:
			MST_air_w = monster_air_right[air].GetWidth();
			MST_air_h = monster_air_right[air].GetHeight();
			monster_air_right[air].Draw(mdc, mst_air[i].x + ss_x, mst_air[i].y + ss_y + landing_shake, 150, 60, 0, 0, MST_air_w, MST_air_h);
			break;
		}
	}
}

//플레이어 이미지 방향 업데이트
void update_player_direction(int mouse_x) {
	if(mouse_x < CM_x + 50) //마우스 좌표가 플레이어보다 왼쪽에 있으면 왼쪽을 바라보고, 오른쪽에 있으면 오른쪽을 바라봄
		CM_img_dir = 0;
	if(mouse_x > CM_x + 50)
		CM_img_dir = 1;
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
		}
	}

	if (is_land == 1) { //점프 후 착지 시 화면에 흔들림 발생
		if (shake_reduce == 0) {
			landing_shake = 30; shake_reduce = 1; //화면 전체가 30씩 내려간다.
		}
		if (shake_reduce == 1) {
			landing_shake -= 2;                   //시간이 지나면서 점차 원래대로 돌아온다
			if (landing_shake == 0) {
				is_land = 0;
				shake_reduce = 0;
			}
		}
	}
	
//좌우 이동
	if (CM_move_dir == 0) { //좌측 이동
		if (BG_scanner >= 10 && CM_x == 700) {  //배경 인식 좌표가 10보다 크고 플레이어 x 좌표가 700이면
			BG_scanner -= 15;					//배경이 움직인다
			for (int i = 0; i < mdx_r; i++)		//몬스터의 위치도 움직인다
				mst_r[i].x += 15;
			for (int i = 0; i < mdx_big; i++)		
				mst_big[i].x += 15;
			for (int i = 0; i < mdx_air; i++)
				mst_air[i].x += 15;
		}

		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2990 && CM_x >= 700)) //배경 인식 좌표가 10이되고 플레이어가 다시 가운데로 이동할 때까지
			CM_x -= 15;                       //플레이어만 움직인다

		if (CM_x <= rt.left)  //벽에 닿으면
			CM_x += 15;                 //이동 중지
	}

	else if (CM_move_dir == 1) { //우측 이동
		if (BG_scanner <= 2990 && CM_x == 700) {
			BG_scanner += 15;
			for (int i = 0; i < mdx_r; i++)
				mst_r[i].x -= 15;
			for (int i = 0; i < mdx_big; i++)
				mst_big[i].x -= 15;
			for (int i = 0; i < mdx_air; i++)
				mst_air[i].x -= 15;
		}

		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2990 && CM_x >= 700))
			CM_x += 15;

		if (CM_x + 100 >= rt.right) 
			CM_x -= 15;
	}
	
}

//몬스터 이미지 방향 업데이트
void update_monster_direction(double CM_x) {
	//일반 몬스터
	for (int i = 0; i < mdx_r; i++)
		update_monster_dir_r(mst_r[i].x + ss_x, CM_x + ss_x, i);

	//대형 몬스터
	for (int i = 0; i < mdx_big; i++)
		update_monster_dir_big(mst_big[i].x + ss_x, CM_x + ss_x, i);

	//공중 몬스터
	for (int i = 0; i < mdx_air; i++)
		update_monster_dir_air(mst_air[i].x + ss_x, CM_x + ss_x, i);
}

//몬스터 위치 업데이트
void update_monster_position() {
	//일반 몬스터 위치
	for (int i = 0; i < mdx_r; i++) {
		if (mst_r[i].img_dir == 0)
			mst_r[i].x -= 5;
		else if (mst_r[i].img_dir == 1)
			mst_r[i].x += 5;
	}

	//대형 몬스터 위치
	for (int i = 0; i < mdx_big; i++) {
		if (mst_big[i].img_dir == 0)
			mst_big[i].x -= 3;
		else if (mst_big[i].img_dir == 1)
			mst_big[i].x += 3;
	}

	//공중 몬스터 위치
	for (int i = 0; i < mdx_air; i++) {
		if (mst_air[i].img_dir == 0)
			mst_air[i].x -= 2;
		else if (mst_air[i].img_dir == 1)
			mst_air[i].x += 2;
	}
}

//몬스터 명중 판정
void check_hit() {
	//일반 몬스터 히트 판정
	for (int i = 0; i < mdx_r; i++) {  
		if (hit_x >= mst_r[i].x && hit_x <= mst_r[i].x + 100 && hit_y >= mst_r[i].y && hit_y <= mst_r[i].y + 100) { 
			hit = i;    //조준점 내부의 좌표가 몬스터 이미지 내부에 위치하면 히트로 판정되어 총알은 해당 몬스터에 가로막힌다.
			angle = atan2(hit_y - (CM_y + 60), hit_x - (CM_x + 50));
			ammo_x2 = ammo_x1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * cos(angle);
			ammo_y2 = ammo_y1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * sin(angle);

			//총마다 대미지를 다르게 줌
			mst_r[hit].hp = cal_damage(mst_r[hit].hp, GUN_number);

			if (mst_r[hit].hp <= 0) {  
				//중간 인덱스를 가진 몬스터를 처치할 경우 나머지 몬스터들의 인덱스가 한 칸씩 앞당겨지고 인덱스 1 감소시킨다.
				if (hit < mdx_r - 1) {
					monster_array_push_r(hit, mdx_r); experience += 5; prev_up = 5; exp_up = TRUE;
				}

				//최신 인덱스를 가진 몬스터의 경우 그냥 인덱스를 감소시킨다.
				else if (hit == mdx_r - 1) {
					mdx_r--; experience += 5; prev_up = 5; exp_up = TRUE;
				}
			}
		}
	}

	//대형 몬스터 히트 판정
	for (int i = 0; i < mdx_big; i++) {
		if (hit_x >= mst_big[i].x && hit_x <= mst_big[i].x + 200 && hit_y >= mst_big[i].y && hit_y <= mst_big[i].y + 200) {
			hit = i;
			angle = atan2(hit_y - (CM_y + 60), hit_x - (CM_x + 50));
			ammo_x2 = ammo_x1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * cos(angle);
			ammo_y2 = ammo_y1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * sin(angle);
			mst_big[hit].hp = cal_damage(mst_big[hit].hp, GUN_number);
			if (mst_big[hit].hp <= 0) {
				if (hit < mdx_big - 1) {
					monster_array_push_big(hit, mdx_big); experience += 7; prev_up = 7; exp_up = TRUE;
				}
				if (hit == mdx_big - 1) {
					mdx_big--; experience += 7; prev_up = 7; exp_up = TRUE;
				}
			}
		}
	}

	//공중 몬스터 히트 판정
	for (int i = 0; i < mdx_air; i++) {
		if (hit_x >= mst_air[i].x && hit_x <= mst_air[i].x + 150 && hit_y >= mst_air[i].y && hit_y <= mst_air[i].y + 200) {
			hit = i; 
			angle = atan2(hit_y - (CM_y + 60), hit_x - (CM_x + 50));
			ammo_x2 = ammo_x1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * cos(angle);
			ammo_y2 = ammo_y1 + cal_dist(CM_x + 50, CM_y + 60, hit_x, hit_y) * sin(angle);
			mst_air[hit].hp = cal_damage(mst_air[hit].hp, GUN_number);
			if (mst_air[hit].hp <= 0) {
				if (hit < mdx_air - 1) {
					monster_array_push_air(hit, mdx_air); experience += 3; prev_up = 3; exp_up = TRUE;
				}
				if (hit == mdx_air - 1) {
					mdx_air--; experience += 3; prev_up = 3; exp_up = TRUE;
				}
			}
		}
	}
}

//사격 시 화면 흔들림 생성
void make_shake(int shake_acc, int shake_end) {
	std::random_device rd_shake;
	std::mt19937 gen(rd_shake());
	std::uniform_int_distribution<int> shake_x(-shake_acc, shake_acc); //숫자가 클 수록 반동이 커 화면 흔들림이 격해짐
	std::uniform_int_distribution<int> shake_y(-shake_acc, shake_acc); //짧은 순간에 ss_x, ss_y에 랜덤한 값을 넣어 흔들림 표현
	ss_x = shake_x(gen); ss_y = shake_y(gen);
	shake_count++;          //몇 번 카운트를 세냐에 따라 화면 흔들리는 시간이 달라진다. 많이 세면 샐 수록 흔들리는 시간이 길어진다.
	shake_acc -= 2;          //변경되는 좌표의 크기가 점차 줄면서 좀 더 자연스러운 흔들림을 만듬
	if (shake_count == shake_end) { //특정 횟수가 되면
		shake_count = 0;    //카운트 초기화
		ss_x = 0; ss_y = 0; //화면 좌표는 원래대로 돌아온다
		shake_effect = 0;   //더 이상 흔들리지 않는다.
		shake_acc = 10;     //흔들림 가속값 다시 초기화
	}
}

//총알 발사 시 수행되는 작업 모음
void make_rand_ammo(int ammo, int max_ammo) {
	if (ammo + 1 == max_ammo) empty = 1;							//총알 소모가 정히진 값에 도달하면 더 이상 발사되지 않는다

	std::random_device rd_ammo;
	std::mt19937 gen(rd_ammo());
	std::uniform_int_distribution<int> x(mx - (10 + var), mx + (10 + var)); //분산도가 넓어질수록 정확도가 떨어지게됨
	std::uniform_int_distribution<int> y(my - (10 + var), my + (10 + var));
	hit_x = x(gen); hit_y = y(gen);									//이 좌표가 몬스터의 이미지 안쪽에 위치해야 대미지 판정을 받는다.
	angle = atan2(y(gen) - (CM_y + 60), x(gen) - (CM_x + 50));      //atan2 함수로 총알이 그려지는 각도를 계산한다.
	ammo_x1 = CM_x + 50; ammo_y1 = CM_y + 60;
	ammo_x2 = ammo_x1 + (1500 * cos(angle));
	ammo_y2 = ammo_y1 + (1500 * sin(angle));

	//히트 판정
	check_hit();

	is_draw = TRUE; draw_hit = TRUE; //그리기 시작
	ind_effect = 1; shake_effect = 1; //각각 인터페이스 이펙트, 흔들림 이펙트
	shoot_delay = 0;	//딜레이는 0이되어 다시 딜레이가 증가하기 시작
}

//사격
void shoot() {
	if (is_click == TRUE && reload == 0 && empty == 0) {
		switch (GUN_number) {
		case scar_h:
			shoot_delay++;
			if (shoot_delay == 6) {
				make_rand_ammo(ammo, 30); //최대 장탄수를 오른쪽 인수에 적는다
				var += 4; ammo++;
			}
			break;
		}
	}
}

//사격 애니메이션 업데이트
void update_shoot_animation() {
	 //사격 시 발생하는 애니매이션
	{
		//사격 시 화면 흔들림
		//좌측 값: 흔들리는 정도, 오른쪽 값: 흔들리는 시간
		if (shake_effect == 1) {
			switch (GUN_number) {
			case scar_h:
				make_shake(10, 5);
				break;
			}
		}

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

		//사격 중지 시 조준점이 다시 회복됨
		if (is_click == FALSE || reload == 1 || empty == 1) {
			if (var >= 0) var -= 2;
		}

		//인터페이스 애니메이션 활성화
		if (ind_effect == 1) { 
			ind_ani = 1; ind_size = 150; ind_x = 1335; ind_y = 625;
			ind_effect = 0;
		}

		//장탄수, 재장전 인디케이터 애니메이션
		if (ind_ani == 1) {
			ind_size -= 10; ind_x += 5; ind_y += 5;

			if (ind_size == 100) ind_ani = 0;
		}

		//재장전 인디케이터
		if (reload == 1) { 
			switch (GUN_number) {
			case scar_h:
				reload_x += 3;
				if (reload_x + CM_x >= CM_x + 100) {
					ammo = 0;  reload = 0;  r_pressed = 0; reload_x = 0; empty = 0;
				}
				break;
			}
		}

		//경험치 증가 애니메이션
		if (exp_up == TRUE) {
			if (out == 1) {
				if(exp_acc > 0)
					exp_x += exp_acc--;
				if (exp_acc == 0) {
					exp_out_delay++;
					if (exp_out_delay == 20) {
						exp_out_delay = 0;
						out = 0;
						exp_acc = 20;
					}
				}
			}

			else if (out == 0) {
				if(exp_acc > 0)
					exp_x -= exp_acc--;
				if (exp_acc == 0) {
					exp_up = FALSE;
					out = 1;
					exp_acc = 20;
				}
			}
		}
	}
}



//WM_KEYDOWN
void wm_keydown() {
	//좌측 이동
	if (GetAsyncKeyState('A') & 0x8000)  CM_move_dir = 0;

	//우측 이동
	else if (GetAsyncKeyState('D') & 0x8000)   CM_move_dir = 1;

	//재장전
	if (GetAsyncKeyState('R') & 0x8000)
		if (r_pressed == 0) {
			reload = 1; r_pressed = 1;
		}

	//점프
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && space_pressed == 0)
		if (space_pressed == 0) {
			CM_jump = 1; space_pressed = 1;
		}
}

//LBUTTONDOWN
void wm_lbuttondown() {
	is_click = TRUE;
	if (empty == 1) reload = 1;

	//연사력이 높을수록 딜레이 수치는 낮음
	//타이머에서 검사하는 딜레이 수치보다 1 적게 초기화
	switch (GUN_number) {
	case scar_h:
		shoot_delay = 5;  break;
	}
}

//몬스터 애니메이션
void monster_animation() {
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
}

//그리기 파트
void wm_paint(HDC mdc, RECT rt) {
	//////////////////////// 버퍼
	BG_w = 1500; BG_h = BackGround.GetHeight();
	BackGround.Draw(mdc, rt.left + ss_x, rt.top - 30 + landing_shake + ss_y, rt.right, rt.bottom + 30, BG_scanner, 0, BG_w, BG_h);
	//BG_scanner가 클수록 배경은 오른쪽으로 이동하게 됨

	//몬스터 이미지 출력
	show_monster(mdc, ss_x, ss_y, landing_shake);

	//총알 궤적 그리기
	if (is_draw == TRUE) draw_ammo(mdc, ammo_x1, ammo_y1, ammo_x2, ammo_y2);

	//플레이어 이미지 출력
	show_player(mdc);

	//히트 포인트 그리기
	if (draw_hit == TRUE) show_hit(mdc, ammo_x2, ammo_y2);

	//인터페이스 출력
	show_interface(mdc, rt);

	//조준점 출력
	if (!manager.isPaused()) show_target(mdc, mx + ss_x, my + ss_y + landing_shake, var);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, mdc;  PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt;

	switch(uMsg) {
	case WM_CREATE:
		IMG_FILE_LOAD();                  //이미지 로드 함수
		CM_x = 700, CM_y = 600;           //초기 플레이어 위치
		SetTimer(hWnd, KEYDOWN, 0, NULL); //KEYDOWN 전용 타이머, 이 타이머에 키보드 입력을 제외한 어떠한 다른것도 작성하지 말 것!
		SetTimer(hWnd, UPDATE, 5, NULL);  //게임 전체 타이머, 추후 애니메이션 전용 타이머도 추가 예정
		break;

	case WM_ACTIVATE:
		switch(LOWORD(wParam)) {
		case WA_INACTIVE:
			manager.interrupt(); break;
		}
		break;

	case WM_KEYDOWN:
		manager.keyboardInput(hWnd, wParam); break;

		//정지상태로 변경
	case WM_KEYUP:
		CM_move_dir = -1; break;

	case WM_LBUTTONDOWN:
		if (manager.getCurrentSceneID() == Game && !manager.isPaused()) wm_lbuttondown();
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Left);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
		//딜레이 수치가 0으로 초기화
		shoot_delay = 0;  
		break;

	case WM_RBUTTONDOWN:
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Right);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam); my = HIWORD(lParam);
		if(manager.getCurrentSceneID() == Game && !manager.isPaused())  update_player_direction(mx);
		break;

	case WM_TIMER:
		switch(wParam) {
		case KEYDOWN: //키보드 입력 전용 타이머. 이동과 점프를 동시에 할 수 있음.
			if (manager.getCurrentSceneID() == Game && !manager.isPaused()) wm_keydown();
			break;

		case UPDATE: //게임 전체 타이머
			GetClientRect(hWnd, &rt);
			manager.update(hWnd);

			if(manager.getCurrentSceneID() == Game && !manager.isPaused()) {
				update_player_position(rt);
				shoot();
				update_shoot_animation();
				update_monster_direction(CM_x);
				update_monster_position();     
				make_monster(rt);  
				monster_animation();
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

			if (manager.getCurrentSceneID() == Game) wm_paint(mdc, rt);

			manager.syncSize(hWnd); manager.show(mdc);
			
			BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
		
			DeleteDC(mdc); DeleteObject(hbitmap); EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		DestroyWindow(hWnd); PostQuitMessage(0);
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_EX_TOPMOST | WS_THICKFRAME, 100, 50, 1500, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}