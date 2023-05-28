#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <iostream>
#include <random>
#include "ammo.h"

enum gun_name_ammo {
	null, scar_h, m16, mp_44, mg_42, awp
};

void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2) { //�Ѿ� ���� �׸���
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 7, RGB(255, 212, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, x1, y1, NULL);
	LineTo(mdc, x2, y2);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

void ammo_indicator(HDC mdc, int ammo, int GUN_number, int ind_size, int ind_x, int ind_y) { //��ź�� ǥ�ñ�
	int ammo_empty = 0;
	TCHAR lpout[50];
	HFONT hfont, oldfont;
	hfont = CreateFont(ind_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���� ���"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(255, 212, 0));

	switch (GUN_number) { //�ִ� ��ź������ ����� ź�� ���� �� ���� ���� ��ź��
	case scar_h:
		if ((25 - ammo) / 10 > 0 && 25 - ammo > 0) {       //�Ѿ��� �� �ڸ��� ������ ��
			wsprintf(lpout, L"%d", 25 - ammo);
			TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
		}
		else if ((25 - ammo) / 10 == 0 && 25 - ammo > 0) { //�Ѿ��� �� �ڸ��� ������ ��
			wsprintf(lpout, L" %d", 25 - ammo);
			TextOut(mdc, ind_x, ind_y, lpout, lstrlen(lpout));
		}

		if (25 - ammo == 0)                                //�Ѿ� ��� �Ҹ� �� ������ ǥ�� 
			ammo_empty = 1;

		break;
	}
	SelectObject(mdc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFont(ind_size, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���� ���"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(255, 0, 0));

	if (ammo_empty == 1) //�Ѿ� ��� �Ҹ� �� ������ ǥ�� 
		TextOut(mdc, ind_x, ind_y, L" R", lstrlen(L" R"));

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}

void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4) { //������ ǥ�ñ�
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