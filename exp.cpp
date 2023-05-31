#include <windows.h>
#include "exp.h"

//°æÇèÄ¡ ¼öÄ¡ Ç¥½Ã ÇÔ¼ö
void show_exp(HDC mdc, int experience, int x, int y) {
	HFONT hfont, oldfont;
	TCHAR exp_tchar[1000] = { 0 };
	hfont = CreateFont(100, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("¸¼Àº °íµñ"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(0, 0, 0));

	wsprintf(exp_tchar, L"%d", experience);
	TextOut(mdc, x, y, exp_tchar, lstrlen(exp_tchar));

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}

void show_exp_add(HDC mdc, int prev_up, int x, int y) {
	HFONT hfont, oldfont;
	TCHAR exp_tchar[10] = { 0 };
	hfont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("¸¼Àº °íµñ"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(0, 0, 0));

	wsprintf(exp_tchar, L"EXP +%d", prev_up);
	TextOut(mdc, x, y, exp_tchar, lstrlen(exp_tchar));

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}