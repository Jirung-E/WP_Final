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
#include "images.h"      //이미지 정보 헤더. 배경, 일부 몬스터, 일부 무기를 제외한 나머지 이미지의 크기는 
                         //모두 100 x 100 px로 하도록 함. 이는 작업 시 좀 더 직관적으로 작업하기 위한 것임.
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

std::random_device rd_main;
std::mt19937 gen(rd_main());

GameManager manager;
extern monster_info_regular mst_r[100]; //몬스터 정보를 저정하는 구조체, monster_info.h에 선언
extern monster_info_big mst_big[100];

enum Timer {
	KEYDOWN, UPDATE
};

//마우스 좌표
static double mx, my;   

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

	SCAR_H_right.Load(L".\\res\\SCAR_H_right.png");
	SCAR_H_left.Load(L".\\res\\SCAR_H_left.png");

	indicator_back.Load(L".\\res\\indicator_back.png");
	ammo_icon.Load(L".\\res\\ammo_icon.png");
} 

//조준점 출력
void show_target(HDC mdc, int mouse_x, int mouse_y, double var) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, mouse_x + 10 + var, mouse_y, NULL);
	LineTo(mdc, mouse_x + 30 + var, mouse_y);
	MoveToEx(mdc, mouse_x - 10 - var, mouse_y, NULL);
	LineTo(mdc, mouse_x - 30 - var, mouse_y);
	MoveToEx(mdc, mouse_x, mouse_y - 10 - var, NULL);
	LineTo(mdc, mouse_x, mouse_y - 30 - var);
	MoveToEx(mdc, mouse_x, mouse_y + 10 + var, NULL);
	LineTo(mdc, mouse_x, mouse_y + 30 + var);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

//인터페이스 출력
void show_interface(HDC mdc, RECT rt) {
	//장탄수 표시기 배경
	IND_w = indicator_back.GetWidth();
	IND_h = indicator_back.GetHeight();
	indicator_back.Draw(mdc, rt.right - 600 + ss_x, rt.bottom - 110 + landing_shake + ss_y, 600, 110, 0, 0, IND_w, IND_h);

	//총 아이콘
	switch (GUN_number) {
	case scar_h:
		GUN_w = SCAR_H_right.GetWidth();
		GUN_h = SCAR_H_right.GetWidth();
		SCAR_H_right.Draw(mdc, rt.right - 430 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 150, 150, 0, 0, GUN_w, GUN_h);
		break;
	}

	//총알 아이콘
	AMO_w = ammo_icon.GetWidth();
	AMO_h = ammo_icon.GetHeight();
	ammo_icon.Draw(mdc, rt.right - 260 + ss_x, rt.bottom - 108 + landing_shake + ss_y, 100, 100, 0, 0, AMO_w, AMO_h);

	//장탄 수 출력
	switch (GUN_number) { //ammo.h에 선언
	case scar_h:
		ammo_indicator(mdc, ammo, scar_h, ind_size, ind_x + ss_x, ind_y + landing_shake + ss_y); //ammo.h에 선언
		break;
	}

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
}

//플레이어 이미지, 총 이미지 출력
void show_player(HDC mdc) {
	switch (CM_img_dir) { //플레이어, 총 이미지 출력
	case 0:
		if (CM_jump == 0) {
			CM_w = commando_left.GetWidth();
			CM_h = commando_left.GetHeight();
			commando_left.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			CM_w = commando_jump_left.GetWidth();
			CM_h = commando_jump_left.GetHeight();
			commando_jump_left.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
		}

		switch (GUN_number) {
		case scar_h:
			GUN_w = SCAR_H_left.GetWidth();
			GUN_h = SCAR_H_left.GetWidth();
			SCAR_H_left.Draw(mdc, CM_x - 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break;
		}

		break;
		//////////////////////
	case 1:
		if (CM_jump == 0) {
			CM_w = commando_right.GetWidth();
			CM_h = commando_right.GetHeight();
			commando_right.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			CM_w = commando_jump_right.GetWidth();
			CM_h = commando_jump_right.GetHeight();
			commando_jump_right.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
		}

		switch (GUN_number) {
		case scar_h:
			GUN_w = SCAR_H_right.GetWidth();
			GUN_h = SCAR_H_right.GetWidth();
			SCAR_H_right.Draw(mdc, CM_x + 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h);
			break;
		}

		break;
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
		}

		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2900 && CM_x >= 700)) //배경 인식 좌표가 10이되고 플레이어가 다시 가운데로 이동할 때까지
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
		}

		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2900 && CM_x >= 700))
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
}

//몬스터 명중 판정
void check_hit() {
	int hit = 0;

	//일반 몬스터 히트 판정
	for (int i = 0; i < mdx_r; i++) {  
		if (hit_x >= mst_r[i].x && hit_x <= mst_r[i].x + 100 && hit_y >= mst_r[i].y && hit_y <= mst_r[i].y + 100) { 
			hit = i;    //조준점 내부의 좌표가 몬스터 이미지 내부에 위치하면 히트로 판정

			//총알은 해당 몬스터에 가로막힌다.
			std::uniform_int_distribution<int> x((mst_r[hit].x + 50) - (10 + var), (mst_r[hit].x + 50) + (10 + var));
			std::uniform_int_distribution<int> y((mst_r[hit].y + 50) - (10 + var), (mst_r[hit].y + 50) + (10 + var));
			angle = atan2(y(gen) - (CM_y + 60), x(gen) - (CM_x + 50));
			ammo_x1 = CM_x + 50;  ammo_y1 = CM_y + 60;
			
			if (CM_img_dir == 0) {
				ammo_x2 = ammo_x1 + (((CM_x + 50) - (mst_r[hit].x + 50)) * cos(angle));
				ammo_y2 = ammo_y1 + (((CM_x + 50) - (mst_r[hit].x + 50) ) * sin(angle));
			}
			else if (CM_img_dir == 1) {
				ammo_x2 = ammo_x1 + (((mst_r[hit].x + 50) - (CM_x + 50)) * cos(angle));
				ammo_y2 = ammo_y1 + (((mst_r[hit].x + 50) - (CM_x + 50)) * sin(angle));
			}
			

			//총마다 대미지를 다르게 줌
			switch (GUN_number) {	   
			case scar_h:
				mst_r[hit].hp -= 8; break;
			}
			
			//몬스터의 hp가 0이 되면 보이지 않는 곳으로 이동된다. 몬스터의 인덱스는 라운드가 올라가거나 게임 오버 시 초기화 된다.
			if (mst_r[hit].hp <= 0) {  
				mst_r[hit].x = -200; mst_r[hit].y = -200;
				mst_r[hit].hp = 0; mst_r[hit].img_dir = -1;
				mst_r[hit].move_dir = -1;

				//중간 인덱스를 가진 몬스터를 처치할 경우 나머지 몬스터들의 인덱스가 한 칸씩 앞당겨지고 인덱스 1 감소시킨다.
				if (hit < mdx_r - 1) {
					for (int i = hit; i < mdx_r - 1; i++) {
						mst_r[i].x = mst_r[i + 1].x;
						mst_r[i].y = mst_r[i + 1].y;
						mst_r[i].hp = mst_r[i + 1].hp;
						mst_r[i].img_dir = mst_r[i + 1].img_dir;
						mst_r[i].move_dir = mst_r[i + 1].move_dir;
					}
					mdx_r--;
				}

				//최신 인덱스를 가진 몬스터의 경우 그냥 인덱스를 감소시킨다.
				else if (hit == mdx_r - 1)
					mdx_r--;
			}
		}
	}

	//대형 몬스터 히트 판정
	for (int i = 0; i < mdx_big; i++) {
		if (hit_x >= mst_big[i].x && hit_x <= mst_big[i].x + 200 && hit_y >= mst_big[i].y && hit_y <= mst_big[i].y + 200) {
			hit = i;    //조준점 내부의 좌표가 몬스터 이미지 내부에 위치하면 히트로 판정

			//총알은 해당 몬스터에 가로막힌다.
			std::uniform_int_distribution<int> x((mst_big[hit].x + 100) - (10 + var), (mst_big[hit].x + 100) + (10 + var));
			std::uniform_int_distribution<int> y((mst_big[hit].y + 100) - (10 + var), (mst_big[hit].y + 100) + (10 + var));
			angle = atan2(y(gen) - (CM_y + 60), x(gen) - (CM_x + 50));
			ammo_x1 = CM_x + 50;  ammo_y1 = CM_y + 60;

			if (CM_img_dir == 0) {
				ammo_x2 = ammo_x1 + (((CM_x + 50) - (mst_big[hit].x + 100)) * cos(angle));
				ammo_y2 = ammo_y1 + (((CM_x + 50) - (mst_big[hit].x + 100)) * sin(angle));
			}
			else if (CM_img_dir == 1) {
				ammo_x2 = ammo_x1 + (((mst_big[hit].x + 100) - (CM_x + 50)) * cos(angle));
				ammo_y2 = ammo_y1 + (((mst_big[hit].x + 100) - (CM_x + 50)) * sin(angle));
			}


			//총마다 대미지를 다르게 줌
			switch (GUN_number) {
			case scar_h:
				mst_big[hit].hp -= 8; break;
			}

			//몬스터의 hp가 0이 되면 보이지 않는 곳으로 이동된다. 몬스터의 인덱스는 라운드가 올라가거나 게임 오버 시 초기화 된다.
			if (mst_big[hit].hp <= 0) {
				mst_big[hit].x = -400; mst_big[hit].y = -400;
				mst_big[hit].hp = 0; mst_big[hit].img_dir = -1;
				mst_big[hit].move_dir = -1;
				
				//중간 인덱스를 가진 몬스터를 처치할 경우 나머지 몬스터들의 인덱스가 한 칸씩 앞당겨지고 인덱스 1 감소시킨다.
				if (hit < mdx_big - 1) {
					for (int i = hit; i < mdx_big - 1; i++) {
						mst_big[i].x = mst_big[i + 1].x;
						mst_big[i].y = mst_big[i + 1].y;
						mst_big[i].hp = mst_big[i + 1].hp;
						mst_big[i].img_dir = mst_big[i + 1].img_dir;
						mst_big[i].move_dir = mst_big[i + 1].move_dir;
					}
					mdx_big--;
				}

				//최신 인덱스를 가진 몬스터의 경우 그냥 인덱스를 감소시킨다.
				else if (hit == mdx_big - 1)
					mdx_big--;
			}
		}
	}
}

//사격 애니메이션 업데이트
void update_shoot_animation(RECT rt, int mouse_x, int mouse_y, BOOL is_click) {
	//사격
	if (is_click == TRUE && reload == 0) {
		switch (GUN_number) {
		case scar_h: //케이스 넘버에 define한 총 이름을 넣으면 됨
			if (ammo < 25) {
				shoot_delay++;                                         //LBUTTON이 눌려있는 동안 딜레이 값이 계속 증가함
				if (shoot_delay == 6) {                                //딜레이 값이 정해진 값에 도달하면
					std::uniform_int_distribution<int> x(mx - (10 + var), mx + (10 + var)); //분산도가 넓어질수록 정확도가 떨어지게됨
					std::uniform_int_distribution<int> y(my - (10 + var), my + (10 + var));
					hit_x = x(gen); hit_y = y(gen);									//이 좌표가 몬스터의 이미지 안쪽에 위치해야 대미지 판정을 받는다.
					angle = atan2(y(gen) - (CM_y + 60), x(gen) - (CM_x + 50));      //atan2 함수로 총알이 그려지는 각도를 계산한다.
					ammo_x1 = CM_x + 50; ammo_y1 = CM_y + 60;
					ammo_x2 = ammo_x1 + (1500 * cos(angle));
					ammo_y2 = ammo_y1 + (1500 * sin(angle));
					check_hit(); is_draw = TRUE;													//그리기 시작
					
					ammo++; var += 4; ind_effect = 1; shake_effect = 1; //각각 인터페이스 이펙트, 흔들림 이펙트
					shoot_delay = 0;								    //딜레이는 0이되어 다시 딜레이가 증가하기 시작
					if (ammo == 25) empty = 1;							//총알 소모가 정히진 값에 도달하면 더 이상 발사되지 않는다
				}
			}
			break;
		}
	}

	

	 //애니매이션
	{
		//아주 짧은 시간동안 총알의 궤적을 그린다.
		if (is_draw == TRUE) { 
			draw_timer++;
			if (draw_timer == 3) {
				draw_timer = 0; is_draw = FALSE;
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

		//사격 시 화면 흔들림
		if (shake_effect == 1) {
			std::random_device rd; std::mt19937 gen(rd());

			//총기마다 반동 수치가 달라 값을 달리해야 함
			switch (GUN_number) { 
			case scar_h:
				shake_acc = 10; //SCAR_H의 경우 흔들림 가속 수치를 10을 줌
				std::uniform_int_distribution<int> shake_x(-shake_acc, shake_acc); //숫자가 클 수록 반동이 커 화면 흔들림이 격해짐
				std::uniform_int_distribution<int> shake_y(-shake_acc, shake_acc); //짧은 순간에 ss_x, ss_y에 랜덤한 값을 넣어 흔들림 표현
				ss_x = shake_x(gen); ss_y = shake_y(gen);						  
				shake_count++;          //몇 번 카운트를 세냐에 따라 화면 흔들리는 시간이 달라진다. 많이 세면 샐 수록 흔들리는 시간이 길어진다.
				shake_acc -=2;          //변경되는 좌표의 크기가 점차 줄면서 좀 더 자연스러운 흔들림을 만듬
				if (shake_count == 5) { //특정 횟수가 되면
					shake_count = 0;    //카운트 초기화
					ss_x = 0; ss_y = 0; //화면 좌표는 원래대로 돌아온다
					shake_effect = 0;   //더 이상 흔들리지 않는다.
					shake_acc = 10;     //흔들림 가속값 다시 초기화
				}
				break;
			}
		}

		//재장전 인디케이터
		if (reload == 1) { 
			reload_x += 2;
			if (reload_x + CM_x == CM_x + 100) { 
				ammo = 0;  reload = 0;  r_pressed = 0; reload_x = 0; empty = 0;
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, mdc;  PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt;
	static BOOL is_click = FALSE; //마우스 클릭 여부

	switch(uMsg) {
	case WM_CREATE:
		IMG_FILE_LOAD();                  //이미지 로드 함수
		CM_x = 700, CM_y = 600;           //초기 플레이어 위치
		ShowCursor(FALSE);                //커서 대신 조준점 보이기
		SetTimer(hWnd, KEYDOWN, 0, NULL); //KEYDOWN 전용 타이머, 이 타이머에 키보드 입력을 제외한 어떠한 다른것도 작성하지 말 것!
		SetTimer(hWnd, UPDATE, 5, NULL);  //게임 전체 타이머, 추후 애니메이션 전용 타이머도 추가 예정
		break;

	case WM_KEYDOWN:
		manager.keyboardInput(hWnd, wParam);
		break;

	case WM_KEYUP:
		CM_move_dir = -1; //정지상태로 변경
		break;

	case WM_LBUTTONDOWN:
		is_click = TRUE;
		if (empty == 1) {
			reload = 1; break;
		}
		//triggered = true;    //스크롤 배경 출력으로 인해 임시 비활성화

		switch (GUN_number) {  //총마다 연사속도가 다르므로 딜레이 수치를 다르게 줘야함. 연사력이 높을수록 딜레이 수치는 낮음
		case scar_h:
			shoot_delay = 5;  break;  //타이머에서 검사하는 딜레이 수치보다 1 적게 초기화해야 발사됨
		}

		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Left);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
		//triggered = false; //스크롤 배경 출력으로 인해 임시 비활성화
		shoot_delay = 0;     //버튼을 떼면 딜레이 수치가 0으로 초기화 된다.
		break;

	case WM_RBUTTONDOWN:
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Right);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam); my = HIWORD(lParam);
		update_player_direction(mx);
		break;

	case WM_TIMER:
		switch(wParam) {
		case KEYDOWN: //키보드 입력 전용 타이머. 이동과 점프를 동시에 할 수 있음.
		if(GetAsyncKeyState('A') & 0x8000)  //좌측 이동
			CM_move_dir = 0;

		if(GetAsyncKeyState('D') & 0x8000)  //우측 이동
			CM_move_dir = 1;

		if(GetAsyncKeyState('R') & 0x8000)  //재장전
			if(r_pressed == 0) {
				reload = 1; r_pressed = 1; //1이면 입력되지 않음, 중복 입력 방지
			}

		if(GetAsyncKeyState(VK_SPACE) & 0x8000 && space_pressed == 0) {
			if(space_pressed == 0) { //VK_SPACE를 계속 누르고 있을 경우 발생할 수 있는 중복 입력 오류를 방지하기 위함
				CM_jump = 1; space_pressed = 1;   //1이면 VK_SPACE는 입력되지 않음
			}
		}
		break;

		case UPDATE: //게임 전체 타이머
			GetClientRect(hWnd, &rt);
			manager.update(hWnd);

			update_player_position(rt);
			update_shoot_animation(rt, mx, my, is_click);
			update_monster_direction(CM_x);
			update_monster_position();

			//spawn_timer의 수치가 0이 되면 몬스터가 한 마리씩 스폰
			//일반 몬스터
			spawn_timer_r--;
			if (spawn_timer_r == 0) {
				if (mdx_r < 99) {
					spawn_monster_regular(mdx_r, BG_scanner, rt);
					mdx_r++;
				}
					spawn_timer_r = 300;
			}

			//대형 몬스터
			spawn_timer_big--;
			if (spawn_timer_big == 0) {
				if (mdx_big < 99) {
					spawn_monster_big(mdx_big, BG_scanner, rt);
					mdx_big++;
				}
				spawn_timer_big = 300;
			}

			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		{
			GetClientRect(hWnd, &rt);
			hdc = BeginPaint(hWnd, &ps);
			mdc = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			(HBITMAP)SelectObject(mdc, hbitmap); 

			//////////////////////// 버퍼
			BG_w = 1500;
			BG_h = BackGround.GetHeight();
			BackGround.Draw(mdc, rt.left + ss_x, rt.top - 30 + landing_shake + ss_y, rt.right, rt.bottom + 30, BG_scanner, 0, BG_w, BG_h);
			//BG_scanner가 클수록 배경은 오른쪽으로 이동하게 됨

			manager.syncSize(hWnd); //스크롤 배경 출력을 위해 임시로 비활성화
			manager.show(mdc);

			//총알 궤적 그리기
			if(is_draw == TRUE)
				draw_ammo(mdc, ammo_x1, ammo_y1, ammo_x2, ammo_y2);

			//플레이어 이미지 출력
			show_player(mdc);

			//몬스터 이미지 출력
			show_monster(mdc, ss_x, ss_y, landing_shake);

			//조준점 출력
			show_target(mdc, mx + ss_x, my + ss_y + landing_shake, var);

			//인터페이스 출력
			show_interface(mdc, rt);
			
			////////////////////////
			////////////////////////

			BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
		
			DeleteDC(mdc);
			DeleteObject(hbitmap);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_EX_TOPMOST, 100, 50, 1500, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}