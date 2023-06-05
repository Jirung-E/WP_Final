#include <windows.h>
#include "player_info.h"

int space_pressed;       //���� Ű �ߺ��� �����ϱ� ���� ����
int CM_move_dir = -1; // 0: �����̵�, 1: ���� �̵� -1: ����
int CM_jump_acc = 28; //���� �� ���ӵ�
int CM_jump;          //0: ���� �� ��, 1: ���� �� 2: ���� ��

double CM_x, CM_y;    //�÷��̾� ĳ������ ��ġ

bool is_zoom = false;
bool avail_awp = false;

int health = 100; //�÷��̾� �⺻ ü�� 100

//�÷��̾� ü�� ��ġ ǥ��
void player_health(HDC mdc, RECT rt, double ss_x, double ss_y, double landing_shake, int health) {
	TCHAR out[10];
	HFONT hfont, oldfont;
	hfont = CreateFont(60, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���� ���"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(0, 0, 0));
	wsprintf(out, L"%d", health);
	for (int i = -3; i <= 3; i++)
		for (int j = -4; j <= 4; j++)
			TextOut(mdc, rt.left + 320 + ss_x + i, rt.bottom - 60 + ss_y + landing_shake + j, out, lstrlen(out));

	SetTextColor(mdc, RGB(255, 255, 255));

	TextOut(mdc, rt.left + 320 + ss_x, rt.bottom - 60 + ss_y + landing_shake, out, lstrlen(out));
	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}