#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <random>
#include "monster_info.h"

monster_info_regular mst_r[100];
monster_info_big mst_big[100];

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
}

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
}

void update_monster_dir_r(double monster_x, double CM_x, int mdx_r) { //몬스터는 항상 플레이어를 바라본다
	if (mst_r[mdx_r].x < CM_x)
		mst_r[mdx_r].img_dir = 1;  //오른쪽 방향

	else if (mst_r[mdx_r].x > CM_x)
		mst_r[mdx_r].img_dir = 0;  //왼쪽 방향
}

void update_monster_dir_big(double monster_x, double CM_x, int mdx_big) { //몬스터는 항상 플레이어를 바라본다
	if (mst_big[mdx_big].x + 50 < CM_x)
		mst_big[mdx_big].img_dir = 1;  //오른쪽 방향

	else if (mst_big[mdx_big].x + 50 > CM_x)
		mst_big[mdx_big].img_dir = 0;  //왼쪽 방향
}

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