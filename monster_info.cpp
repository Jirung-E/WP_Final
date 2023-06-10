#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <random>
#include "monster_info.h"

monster_info_regular mst_r[100];
monster_info_big mst_big[100];
monster_info_air mst_air[100];
dead_location dl[100];

//라운드 업 애니메이션을 재생하기 위한 변수
BOOL round_up = FALSE;
int kill_count = 0;
int round_size = 70;
int round_x = 630;

//최초 라운드1
int game_round = 1;			// 스폰 주기를 결정함

//몬스터 시체 인덱스
int ddx = 0;
int delete_delay = 0;  //특정 수치가 되면 시체 인덱스를 하나씩 삭제한다.

int mdx_r = 0;                 //일반 몬스터 수 (monster - index_regular)
int spawn_timer_r = 10;   //라운드가 올라갈 수록 스폰 주기가 빨라진다. -> 수치가 낮아진다. 기본 수치는 500부터 시작
const int spawn_timer_r_max = 140;	// 스폰 주기 최초 수치

int mdx_big = 0;				  //대형 몬스터 수
int spawn_timer_big = 10;		// 1000
const int spawn_timer_big_max = 600;

int mdx_air = 0;				  //공중 몬스터 수
int spawn_timer_air = 10;		// 600
const int spawn_timer_air_max = 400;

//awp한정 몬스터를 명중 했는지를 나타내는 변수
BOOL is_hit = FALSE;

int calc_spawn_timer(int spawn_timer_max) {
	if(game_round < 100) {
		return spawn_timer_max * (100.0 - (game_round-1)) / 100;
	}
	else
		return spawn_timer_max;
}

//일반 몬스터 생성
void spawn_monster_regular(int mdx_r, int BG_scanner, RECT rt) {
	int spawn_dir = 0;
	std::random_device rd_mst;
	std::mt19937 gen(rd_mst());
	std::uniform_int_distribution<int> left_or_right(0, 1); //몬스터는 맵 왼쪽 끝 또는 오른쪽 끝에서 생성된다
	spawn_dir = left_or_right(gen);							//0이면 왼쪽, 1이면 오른쪽
	if (spawn_dir == 0) {
		mst_r[mdx_r].x = rt.left - BG_scanner - 100;				//맵의 왼쪽 끝에서 생성
		mst_r[mdx_r].y = 600;	
	}
	else if (spawn_dir == 1) {
		mst_r[mdx_r].x = rt.right + 3000 - BG_scanner + 100;    //맵의 오른쪽 끝에서 생성
		mst_r[mdx_r].y = 600;
	}
	mst_r[mdx_r].hp = 50;
	mst_r[mdx_r].targeted = 0;
	mst_r[mdx_r].height = 100;
	mst_r[mdx_r].motion_acc = 8;
}

//대형 몬스터 생성
void spawn_monster_big(int mdx_big, int BG_scanner, RECT rt) {
	int spawn_dir = 0;
	std::random_device rd_mst;
	std::mt19937 gen(rd_mst());
	std::uniform_int_distribution<int> left_or_right(0, 1); //몬스터는 맵 왼쪽 끝 또는 오른쪽 끝에서 생성된다
	spawn_dir = left_or_right(gen);							//0이면 왼쪽, 1이면 오른쪽
	if (spawn_dir == 0) {
		mst_big[mdx_big].x = rt.left - BG_scanner - 200;				//맵의 왼쪽 끝에서 생성
		mst_big[mdx_big].y = 500;
	}
	else if (spawn_dir == 1) {
		mst_big[mdx_big].x = rt.right + 3000 - BG_scanner + 200;    //맵의 오른쪽 끝에서 생성
		mst_big[mdx_big].y = 500;
	}
	mst_big[mdx_big].hp = 100;
	mst_big[mdx_big].targeted = 0;
	mst_big[mdx_big].height = 200;
	mst_big[mdx_big].motion_acc = 8;
}

//공중 몬스터 생성
void spawn_monster_air(int mdx_air, int BG_scanner, RECT rt) {
	int spawn_dir = 0;
	std::random_device rd_mst;
	std::mt19937 gen(rd_mst());
	std::uniform_int_distribution<int> left_or_right(0, 1); //몬스터는 맵 왼쪽 끝 또는 오른쪽 끝에서 생성된다
	spawn_dir = left_or_right(gen);							//0이면 왼쪽, 1이면 오른쪽
	if (spawn_dir == 0) {
		mst_air[mdx_air].x = rt.left - BG_scanner - 150;				//맵의 왼쪽 끝에서 생성
		mst_air[mdx_air].y = 200;
	}
	else if (spawn_dir == 1) {
		mst_air[mdx_air].x = rt.right + 3000 - BG_scanner + 150;    //맵의 오른쪽 끝에서 생성
		mst_air[mdx_air].y = 200;
	}
	mst_air[mdx_air].hp = 30;
	mst_air[mdx_air].height = 60;
	mst_air[mdx_air].motion_acc = 8;
}

//일반 몬스터 이미지 방향
void update_monster_dir_r(double monster_x, double CM_x, int mdx_r) { 
	if (mst_r[mdx_r].x > CM_x + 4 || mst_r[mdx_r].x < CM_x - 4) {
		if (mst_r[mdx_r].x < CM_x)
			mst_r[mdx_r].img_dir = 1;  //오른쪽 방향

		else if (mst_r[mdx_r].x > CM_x)
			mst_r[mdx_r].img_dir = 0;  //왼쪽 방향
	}
}

//대형 몬스터 이미지 방향
void update_monster_dir_big(double monster_x, double CM_x, int mdx_big) {
	if (mst_big[mdx_big].x + 50 > CM_x + 4 || mst_big[mdx_big].x + 50 < CM_x - 4) {
		if (mst_big[mdx_big].x + 50 < CM_x)
			mst_big[mdx_big].img_dir = 1;  //오른쪽 방향

		else if (mst_big[mdx_big].x + 50 > CM_x)
			mst_big[mdx_big].img_dir = 0;  //왼쪽 방향
	}
}

//공중 몬스터 이미지 방향
void update_monster_dir_air(double monster_x, double CM_x, int mdx_air) {
	if (mst_air[mdx_air].x + 25 > CM_x + 4 || mst_air[mdx_air].x + 25 < CM_x - 4) {
		if (mst_air[mdx_air].x + 25 < CM_x)
			mst_air[mdx_air].img_dir = 1;  //오른쪽 방향

		else if (mst_air[mdx_air].x + 25 > CM_x)
			mst_air[mdx_air].img_dir = 0;  //왼쪽 방향
	}
}

//몬스터 체력 표시
void monster_hp_ind(HDC mdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);

	Rectangle(mdc, x3, y3, x4, y4);

	SelectObject(mdc, oldbrush);
	DeleteObject(hbrush);

	hbrush = CreateSolidBrush(RGB(255, 0, 0));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);

	Rectangle(mdc, x1, y1, x2, y2);

	SelectObject(mdc, oldbrush);
	DeleteObject(hbrush);

	hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);
	hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	Rectangle(mdc, x3, y3, x4, y4);

	SelectObject(mdc, oldbrush);
	DeleteObject(hbrush);
	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

//일반 몬스터 배열 밀어내기
void monster_array_push_r(int hit, int idx) {
	for (int i = hit; i < idx; i++) {
		mst_r[i].x = mst_r[i + 1].x;
		mst_r[i].y = mst_r[i + 1].y;
		mst_r[i].hp = mst_r[i + 1].hp;
		mst_r[i].img_dir = mst_r[i + 1].img_dir;
		mst_r[i].move_dir = mst_r[i + 1].move_dir;
		mst_r[i].targeted = mst_r[i + 1].targeted;
		mst_r[i].height = mst_r[i + 1].height;
		mst_r[i].motion_acc = mst_r[i + 1].motion_acc;
	}
	idx--;
	mst_r[idx].x = -400;
	mst_r[idx].y = -400;
	mst_r[idx].hp = 0;
	mst_r[idx].move_dir = -1;
	mst_r[idx].img_dir = -1; //현재 인덱스의 바로 전 인덱스까지 모두 한 칸씩 앞으로 당긴 후 남는 인덱스의 몬스터는 화면 밖으로 이동(나중에 몬스터 스폰 시 다시 값이 생성되므로 문제 없음)
	mst_r[idx].targeted = 0;
	mst_r[idx].height = 0;
	mst_r[idx].motion_acc = 0;
}

//대형 몬스터 배열 밀어내기
void monster_array_push_big(int hit, int idx) {
	for (int i = hit; i < idx; i++) {
		mst_big[i].x = mst_big[i + 1].x;
		mst_big[i].y = mst_big[i + 1].y;
		mst_big[i].hp = mst_big[i + 1].hp;
		mst_big[i].img_dir = mst_big[i + 1].img_dir;
		mst_big[i].move_dir = mst_big[i + 1].move_dir;
		mst_big[i].targeted = mst_big[i + 1].targeted;
		mst_big[i].height = mst_big[i + 1].height;
		mst_big[i].motion_acc = mst_big[i + 1].motion_acc;
	}
	idx--;
	mst_big[idx].x = -400;
	mst_big[idx].y = -400;
	mst_big[idx].hp = 0;
	mst_big[idx].move_dir = -1;
	mst_big[idx].img_dir = -1;
	mst_big[idx].targeted = 0;
	mst_big[idx].height = 0;
	mst_big[idx].motion_acc = 0;
}

//공중 몬스터 배열 밀어내기
void monster_array_push_air(int hit, int idx) {
	for (int i = hit; i < idx; i++) {
		mst_air[i].x = mst_air[i + 1].x;
		mst_air[i].y = mst_air[i + 1].y;
		mst_air[i].hp = mst_air[i + 1].hp;
		mst_air[i].img_dir = mst_air[i + 1].img_dir;
		mst_air[i].move_dir = mst_air[i + 1].move_dir;
		mst_air[i].height = mst_air[i + 1].height;
		mst_air[i].motion_acc = mst_air[i + 1].motion_acc;
		
	}
	idx --;
	mst_air[idx].x = -400;
	mst_air[idx].y = -400;
	mst_air[idx].hp = 0;
	mst_air[idx].move_dir = -1;
	mst_air[idx].img_dir = -1;
	mst_air[idx].height = 0;
	mst_air[idx].motion_acc = 0;
}

//시체 인덱스 밀어내기
void push_dead(int idx) {
	for (int i = 0; i < idx - 1; i++) {
		dl[i].x = dl[i + 1].x;
		dl[i].y = dl[i + 1].y;
		dl[i].monster_type = dl[i + 1].monster_type;
		dl[i].acc = dl[i + 1].acc;
		dl[i].dir = dl[i + 1].dir;
		dl[i].motion_dir = dl[i + 1].motion_dir;
	}
}