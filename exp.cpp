#include <windows.h>
#include "exp.h"

//경험치. exp로 변수 이름을 하려 했으나 충돌로 인해 이렇게 함
int experience;
//경험치가 증가하는 정도를 저장하는 변수. 경험치 증가 효과를 출력할 때 사용한다.
int prev_up;
//경험치가 증가할 때 경험치 증가 이벤트를 발생시킨다.
BOOL exp_up = FALSE;
int exp_x = -150;
int exp_acc = 20;
int out = 1;
int exp_out_delay;

//경험치 수치 표시 함수
void show_exp(HDC mdc, int experience, int x, int y) {
	HFONT hfont, oldfont;
	TCHAR exp_tchar[1000] = { 0 };
	hfont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(0, 0, 0));

	wsprintf(exp_tchar, L"%d", experience);
	for (int i = -3; i <= 3; i++)
		for (int j = -4; j <= 4; j++)
			TextOut(mdc, x + i, y + j, exp_tchar, lstrlen(exp_tchar));
	SetTextColor(mdc, RGB(255, 255, 255));
	TextOut(mdc, x, y, exp_tchar, lstrlen(exp_tchar));

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}

//경험치 획득 수치 표시 함수
void show_exp_add(HDC mdc, int prev_up, int x, int y) {
	HFONT hfont, oldfont;
	TCHAR exp_tchar[10] = { 0 };
	hfont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(0, 0, 0));

	wsprintf(exp_tchar, L"EXP +%d", prev_up);
	for (int i = -3; i <= 3; i++)
		for (int j = -4; j <= 4; j++)
			TextOut(mdc, x + i, y + j, exp_tchar, lstrlen(exp_tchar));
	SetTextColor(mdc, RGB(255, 255, 255));
	TextOut(mdc, x, y, exp_tchar, lstrlen(exp_tchar));


	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}