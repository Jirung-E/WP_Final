#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <iostream>
#include <random>
#include "ammo.h"

ammo_pos ap[500]; //총알의 위치, 날아가는 여부를 저장하는 구조체

std::random_device rd;
std::mt19937 gen(rd());

enum gun_name_ammo {
	null, scar_h, m16, mp_44, mg_42, p90
};

void make_ammo(int apx, double CM_x, double CM_y, double mx, double my, double var) { //총알 객체 생성
	ap[apx].is_shoot = 1; //총알은 날아가는 상태가 된다.
	std::uniform_int_distribution<int> x(mx - (10 + var), mx + (10 + var)); //분산도가 넓어질수록 정확도가 떨어지게됨
	std::uniform_int_distribution<int> y(my - (10 + var), my + (10 + var));
	ap[apx].angle = atan2(y(gen) - (CM_y + 60), x(gen) - (CM_x + 50));      //atan2 함수로 총알이 그려지는 각도를 계산한다.
	//아래 4개의 값들은 선의 좌표
	ap[apx].x = CM_x + 50;
	ap[apx].y = CM_y + 60;
	ap[apx].x2 = ap[apx].x + (50 * cos(ap[apx].angle));
	ap[apx].y2 = ap[apx].y + (50 * sin(ap[apx].angle));                     //ap[apx].x2, ap[apx].y2가 몬스터와 충돌하는 조건에 쓰일 예정
}

void draw_ammo(HDC mdc, double x, double y, double x2, double y2) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 7, RGB(255, 165, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, x, y, NULL);
	LineTo(mdc, x2, y2);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

void ammo_move(int apx, int is_shoot, RECT rt, int BG_scanner) { //총알 애니메이션
	if (ap[apx].is_shoot == 1) {
		ap[apx].x += 100 * cos(ap[apx].angle); //make_ammo에서 구한 각도로 날아간다
		ap[apx].y += 100 * sin(ap[apx].angle);
		ap[apx].x2 += 100 * cos(ap[apx].angle);
		ap[apx].y2 += 100 * sin(ap[apx].angle);

		if (ap[apx].x >= rt.right + (2990 - BG_scanner) || ap[apx].x <= rt.left - BG_scanner) { //총알 뒤쪽의 x좌표가 맵 왼쪽이나 오른쪽을 넘어가면
			ap[apx].x = -10;                                                                    //총알 객체는 윈도우 밖으로 이동되고 더 이상 날아가지 않는다
			ap[apx].y = -10;																	//(실제로는 안보이는 곳으로 치운다)
			ap[apx].x2 = -10;
			ap[apx].y2 = -10;
			ap[apx].angle = NULL;
			ap[apx].is_shoot = 0;
		}
	}
}

void ammo_indicator(HDC mdc, int apx, int GUN_number, int ind_size, int ind_x, int ind_y) { //장탄수 표시기
	int ammo_empty = 0;
	TCHAR lpout[50];
	HFONT hfont, oldfont;
	hfont = CreateFont(ind_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(255, 212, 0));

	switch (GUN_number) { //최대 장탄수에서 총알 인덱스를 뺀 값이 현재 장탄수
	case scar_h:
		if ((25 - apx) / 10 > 0 && 25 - apx > 0) {       //총알이 두 자릿수 남았을 때
			wsprintf(lpout, L"%d", 25 - apx);
			TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
		}
		else if ((25 - apx) / 10 == 0 && 25 - apx > 0) { //총알이 한 자릿수 남았을 때
			wsprintf(lpout, L" %d", 25 - apx);
			TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
		}

		if (25 - apx == 0)                               //총알 모두 소모 시 재장전 표시 
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

void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4) { //재장전 표시기
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	hbrush = CreateSolidBrush(RGB(255, 0, 0));
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
	SelectObject(mdc, hpen);
	DeleteObject(oldpen);
}