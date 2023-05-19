#include <windows.h>
#include <tchar.h>
#include "target.h"

void draw_target(HDC mdc, double mx, double my, double var) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, mx + 10 + var, my, NULL);
	LineTo(mdc, mx + 30 + var, my);
	MoveToEx(mdc, mx - 10 - var, my, NULL);
	LineTo(mdc, mx - 30 - var, my);
	MoveToEx(mdc, mx, my - 10 - var, NULL);
	LineTo(mdc, mx, my - 30 - var);
	MoveToEx(mdc, mx, my + 10 + var, NULL);
	LineTo(mdc, mx, my + 30 + var);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}