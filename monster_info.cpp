#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <random>
#include "monster_info.h"

monster_info_regular mst_r[100];
monster_info_big mst_big[100];
monster_info_air mst_air[100];

//�Ϲ� ���� ����
void spawn_monster_regular(int mdx_r, int BG_scanner, RECT rt) {
	int spawn_dir = 0;
	std::random_device rd_mst;
	std::mt19937 gen(rd_mst());
	std::uniform_int_distribution<int> left_or_right(0, 1); //���ʹ� �� ���� �� �Ǵ� ������ ������ �����ȴ�
	spawn_dir = left_or_right(gen);							//0�̸� ����, 1�̸� ������
	if (spawn_dir == 0) {
		mst_r[mdx_r].x = rt.left - BG_scanner - 100;				//���� ���� ������ ����
		mst_r[mdx_r].y = 600;	
	}
	else if (spawn_dir == 1) {
		mst_r[mdx_r].x = rt.right + 3000 - BG_scanner + 100;    //���� ������ ������ ����
		mst_r[mdx_r].y = 600;
	}
	mst_r[mdx_r].hp = 50;
}

//���� ���� ����
void spawn_monster_big(int mdx_big, int BG_scanner, RECT rt) {
	int spawn_dir = 0;
	std::random_device rd_mst;
	std::mt19937 gen(rd_mst());
	std::uniform_int_distribution<int> left_or_right(0, 1); //���ʹ� �� ���� �� �Ǵ� ������ ������ �����ȴ�
	spawn_dir = left_or_right(gen);							//0�̸� ����, 1�̸� ������
	if (spawn_dir == 0) {
		mst_big[mdx_big].x = rt.left - BG_scanner - 200;				//���� ���� ������ ����
		mst_big[mdx_big].y = 500;
	}
	else if (spawn_dir == 1) {
		mst_big[mdx_big].x = rt.right + 3000 - BG_scanner + 200;    //���� ������ ������ ����
		mst_big[mdx_big].y = 500;
	}
	mst_big[mdx_big].hp = 100;
}

//���� ���� ����
void spawn_monster_air(int mdx_air, int BG_scanner, RECT rt) {
	int spawn_dir = 0;
	std::random_device rd_mst;
	std::mt19937 gen(rd_mst());
	std::uniform_int_distribution<int> left_or_right(0, 1); //���ʹ� �� ���� �� �Ǵ� ������ ������ �����ȴ�
	spawn_dir = left_or_right(gen);							//0�̸� ����, 1�̸� ������
	if (spawn_dir == 0) {
		mst_air[mdx_air].x = rt.left - BG_scanner - 150;				//���� ���� ������ ����
		mst_air[mdx_air].y = 200;
	}
	else if (spawn_dir == 1) {
		mst_air[mdx_air].x = rt.right + 3000 - BG_scanner + 150;    //���� ������ ������ ����
		mst_air[mdx_air].y = 200;
	}
	mst_air[mdx_air].hp = 30;
}

//�Ϲ� ���� �̹��� ����
void update_monster_dir_r(double monster_x, double CM_x, int mdx_r) { 
	if (mst_r[mdx_r].x < CM_x)
		mst_r[mdx_r].img_dir = 1;  //������ ����

	else if (mst_r[mdx_r].x > CM_x)
		mst_r[mdx_r].img_dir = 0;  //���� ����
}

//���� ���� �̹��� ����
void update_monster_dir_big(double monster_x, double CM_x, int mdx_big) {
	if (mst_big[mdx_big].x + 50 < CM_x)
		mst_big[mdx_big].img_dir = 1;  //������ ����

	else if (mst_big[mdx_big].x + 50 > CM_x)
		mst_big[mdx_big].img_dir = 0;  //���� ����
}

//���� ���� �̹��� ����
void update_monster_dir_air(double monster_x, double CM_x, int mdx_air) {
	if (mst_air[mdx_air].x + 75 < CM_x)
		mst_air[mdx_air].img_dir = 1;  //������ ����

	else if (mst_air[mdx_air].x + 75 > CM_x)
		mst_air[mdx_air].img_dir = 0;  //���� ����
}

//���� ü�� ǥ��
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

//�Ϲ� ���� �迭 �о��
void monster_array_push_r(int hit, int idx) {
	for (int i = hit; i < idx; i++) {
		mst_r[i].x = mst_r[i + 1].x;
		mst_r[i].y = mst_r[i + 1].y;
		mst_r[i].hp = mst_r[i + 1].hp;
		mst_r[i].img_dir = mst_r[i + 1].img_dir;
		mst_r[i].move_dir = mst_r[i + 1].move_dir;
	}
	idx--;
	mst_r[idx].x = -400;
	mst_r[idx].y = -400;
	mst_r[idx].hp = 0;
	mst_r[idx].move_dir = -1;
	mst_r[idx].img_dir = -1; //���� �ε����� �ٷ� �� �ε������� ��� �� ĭ�� ������ ��� �� ���� �ε����� ���ʹ� ȭ�� ������ �̵�(���߿� ���� ���� �� �ٽ� ���� �����ǹǷ� ���� ����)
}

//���� ���� �迭 �����
void monster_array_push_big(int hit, int idx) {
	for (int i = hit; i < idx; i++) {
		mst_big[i].x = mst_big[i + 1].x;
		mst_big[i].y = mst_big[i + 1].y;
		mst_big[i].hp = mst_big[i + 1].hp;
		mst_big[i].img_dir = mst_big[i + 1].img_dir;
		mst_big[i].move_dir = mst_big[i + 1].move_dir;
	}
	idx--;
	mst_big[idx].x = -400;
	mst_big[idx].y = -400;
	mst_big[idx].hp = 0;
	mst_big[idx].move_dir = -1;
	mst_big[idx].img_dir = -1;
}

//���� ���� �迭 �о��
void monster_array_push_air(int hit, int idx) {
	for (int i = hit; i < idx; i++) {
		mst_air[i].x = mst_air[i + 1].x;
		mst_air[i].y = mst_air[i + 1].y;
		mst_air[i].hp = mst_air[i + 1].hp;
		mst_air[i].img_dir = mst_air[i + 1].img_dir;
		mst_air[i].move_dir = mst_air[i + 1].move_dir;
	}
	idx --;
	mst_air[idx].x = -400;
	mst_air[idx].y = -400;
	mst_air[idx].hp = 0;
	mst_air[idx].move_dir = -1;
	mst_air[idx].img_dir = -1;
}