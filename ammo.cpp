#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <iostream>
#include <random>
#include "ammo.h"
#include "gun_info.h"

enum gun_name_ammo {
	null, scar_h, m16, mp_44, mg_42, awp
};

BOOL is_draw = FALSE;					  //TRUE일 시 총알 궤적을 그림
int ammo;							      //장탄 수
int draw_timer;							  //궤적이 그려지는 시간을 정한다. 수치가 높을수록 더 오랫동안 그려진다.
double angle;							  //총알 궤적의 각도
double ammo_x1, ammo_y1, ammo_x2, ammo_y2; //총알 궤적이 그려지는 좌표
double dist;//플레이어와 히트 판정점 사이의 거리

BOOL draw_hit = FALSE;					  //TRUE일 시 히트 포인트를 그림
int hit_timer;							  //히트 포인트가 그려지는 시간	
int hit;									  //히트 판정을 받은 몬스터의 인덱스
double hit_x, hit_y;   //조준점 내부에서 생성되는 실제 x,y 좌표

double var;            //총을 오래 사격할 수록 반동으로 인해 정확도가 떨어짐, 수치가 증가할 수록 분산도가 커짐

int ind_size = 100; //ammo_indicator의 폰트 크기
int ind_x = 1360;   //ammo_indicator의 폰트 위치
int ind_y = 650;    //ammo_indicator의 폰트 위치
int ind_ani;        //총알이 소모 될 때마다 애니메이션 재생, 0: 재생 안함, 1: 재생

int shoot_delay; //연사 속도 조절 변수 //수치가 높을수록 연사가 느려짐
int r_pressed;      //재장전 중복 입력 방지를 위한 변수
int reload;      //0이면 재장전 안함, 1이면 재장전 중
int reload_x;    //재장전 표시기 x2 좌표

int empty;   //총알 모두 소모 시 1로 변경

BOOL can_shoot = TRUE; //연사 속도가 느린 총을 마우스 연타하여 더 빠르게 쏘는 부정 행위 방지
int after_delay;       //이 변수가 연사 속도 수치와 다르면 마우스를 아무리 빨리 눌러도 발사가 되지 않는다.

int reload_delay; //mg_42 재장전 속도 조정 변수

ellipse e[100];
int edx = 0;

double incline;
double rect_x_end, rect_y_end;
double rect_x_start, rect_y_start;
int rect_size_x = 20, rect_size_y = 20;
int rect_dir;
BOOL is_lazer = FALSE;
BOOL can_make_rect = TRUE;
BOOL is_rect = FALSE;
int rect_acc;
int rect_delay;
int rect_delete_delay;


//총알 궤적 그리기
void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2, int GUN_number) { 
	if (GUN_number != awp) {
		HPEN hpen, oldpen;
		hpen = CreatePen(PS_SOLID, 7, RGB(255, 212, 0));
		oldpen = (HPEN)SelectObject(mdc, hpen);

		MoveToEx(mdc, x1, y1, NULL);
		LineTo(mdc, x2, y2);

		SelectObject(mdc, oldpen);
		DeleteObject(hpen);
	}

	if (GUN_number == awp) {
		HPEN hpen, oldpen;
		hpen = CreatePen(PS_SOLID, 15, RGB(255, 212, 0));
		oldpen = (HPEN)SelectObject(mdc, hpen);

		MoveToEx(mdc, x1, y1, NULL);
		LineTo(mdc, x2, y2);

		SelectObject(mdc, oldpen);
		DeleteObject(hpen);
	}
}

//장탄수 표시
void ammo_indicator(HDC mdc, int max_ammo, int ammo, int ind_size, int ind_x, int ind_y) {
	int ammo_empty = 0;
	TCHAR lpout[50];
	HFONT hfont, oldfont;
	hfont = CreateFont(ind_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(255, 212, 0));

	if ((max_ammo - ammo) / 100 > 0 && max_ammo - ammo > 0) {        //총알이 세 자릿수 남았을 때
		wsprintf(lpout, L"%d", max_ammo - ammo);
		TextOut(mdc, ind_x - 30, ind_y, lpout, lstrlen(lpout));
	}
	else if ((max_ammo - ammo) / 10 > 0 && max_ammo - ammo > 0) {       //총알이 두 자릿수 남았을 때
		wsprintf(lpout, L"%d", max_ammo - ammo);
		TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
	}
	else if ((max_ammo - ammo) / 10 == 0 && max_ammo - ammo > 0) { //총알이 한 자릿수 남았을 때
		wsprintf(lpout, L" %d", max_ammo - ammo);
		TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
	}

	if (max_ammo - ammo == 0)                                //총알 모두 소모 시 재장전 표시 
		ammo_empty = 1;
	
	SelectObject(mdc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFont(ind_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(255, 0, 0));

	if (ammo_empty == 1) //총알 모두 소모 시 재장전 표시 
		TextOut(mdc, ind_x, ind_y, L" R", lstrlen(L" R"));

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}

//재장전 게이지 표시
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4) {
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);

	Rectangle(mdc, x3, y3, x4, y4);

	SelectObject(mdc, oldbrush);
	DeleteObject(hbrush);

	hbrush = CreateSolidBrush(RGB(255, 255, 255));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);

	Rectangle(mdc, x, y, x2, y2);

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

//플레이어와 히트 포인트의 거리 계산
int cal_dist(double x1, double y1, double x2, double y2) {
	double distance = sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));

	return distance;
}

//총마다 다른 대미지 계산
int cal_damage(int monster_hp, int GUN_number) {
	return monster_hp - Gun::damage(GUN_number);
}

void push_ellipse(int del, int idx) {
	for (int i = del; i < idx; i++) {
		e[i].x = e[i + 1].x;
		e[i].y = e[i + 1].y;
		e[i].acc = e[i + 1].acc;
		e[i].size = e[i + 1].size;
		e[i].dir = e[i + 1].dir;
	}
	idx--;
	e[idx].x = -400;
	e[idx].y = 1500;
	e[idx].size = 0;
	e[idx].acc = 0;
	e[idx].dir = -1;
}
