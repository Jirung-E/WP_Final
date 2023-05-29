#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <iostream>
#include <random>
#include "ammo.h"

enum gun_name_ammo {
	null, scar_h, m16, mp_44, mg_42, awp
};

//총알 궤적 그리기
void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2) { 
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 7, RGB(255, 212, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, x1, y1, NULL);
	LineTo(mdc, x2, y2);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

//장탄수 표시
void ammo_indicator(HDC mdc, int ammo, int GUN_number, int ind_size, int ind_x, int ind_y) {
	int ammo_empty = 0;
	TCHAR lpout[50];
	HFONT hfont, oldfont;
	hfont = CreateFont(ind_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(255, 212, 0));

	switch (GUN_number) { //최대 장탄수에서 사용한 탄약 수를 뺀 값이 현재 장탄수
	case scar_h:
		if ((30 - ammo) / 10 > 0 && 30 - ammo > 0) {       //총알이 두 자릿수 남았을 때
			wsprintf(lpout, L"%d", 30 - ammo);
			TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
		}
		else if ((30 - ammo) / 10 == 0 && 30 - ammo > 0) { //총알이 한 자릿수 남았을 때
			wsprintf(lpout, L" %d", 30 - ammo);
			TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
		}

		if (30 - ammo == 0)                                //총알 모두 소모 시 재장전 표시 
			ammo_empty = 1;

		break;
	}
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
	switch (GUN_number) {
	case scar_h:
		monster_hp -= 8; 
		break;
	}

	return monster_hp;
}
