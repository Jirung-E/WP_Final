#include <windows.h>
#include "player_info.h"

int space_pressed;       //점프 키 중복을 방지하기 위한 변수
int CM_move_dir = -1; // 0: 좌측이동, 1: 우측 이동 -1: 정지
int CM_jump_acc = 28; //점프 시 가속도
int CM_jump;          //0: 점프 안 함, 1: 점프 함 2: 착지 중
int cool_time;

//BOOL로 하면 오류가 나서 int로 대체
int cat_ready = 0;
int awp_cat_delay = 0;

double CM_x, CM_y;    //플레이어 캐릭터의 위치

bool is_zoom = false;
bool avail_awp = false;

int health = 100; //플레이어 기본 체력 100
int recovery_delay = 0; //플레이어 체력 회복 딜레이

//플레이어 체력 수치 표시
void player_health(HDC mdc, RECT rt, double ss_x, double ss_y, double landing_shake, int health) {
	TCHAR out[10];
	HFONT hfont, oldfont;
	hfont = CreateFont(60 / 1500.0 * rt.right, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);
	SetTextColor(mdc, RGB(0, 0, 0));

	int fx = (rt.left + 320 + ss_x) / 1500.0 * rt.right;
	int fy = rt.bottom + (-60 + ss_y + landing_shake) / 1500.0 * rt.right;

	wsprintf(out, L"%d", health);
	for (int i = -3; i <= 3; i++)
		for (int j = -4; j <= 4; j++)
			TextOut(mdc, fx + i, fy + j, out, lstrlen(out));

	SetTextColor(mdc, RGB(255, 255, 255));

	TextOut(mdc, fx, fy, out, lstrlen(out));
	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}

//수류탄 대기 시간 출력
void show_gren_time(HDC mdc, double ss_x, double ss_y, double landing_shake, int gren_time, RECT rt) {
	TCHAR out[100] = { 0 };
	HFONT hfont, oldfont;
	hfont = CreateFont(60 / 1500.0 * rt.right, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);

	int fx = (500 + ss_x) / 1500.0 * rt.right;
	int fy = rt.bottom + (-70 + ss_y + landing_shake) / 1500.0 * rt.right;
	
	if (gren_time > 1) {
		wsprintf(out, L"%d", gren_time);
		SetTextColor(mdc, RGB(0, 0, 0));
		for (int i = -3; i <= 3; i++)
			for (int j = -4; j <= 4; j++)
				TextOut(mdc, fx + i, fy + j, out, lstrlen(out));
		SetTextColor(mdc, RGB(255, 255, 255));
		TextOut(mdc, fx, fy, out, lstrlen(out));
	}
	else if (gren_time == 0) {
		SetTextColor(mdc, RGB(0, 0, 0));
		for (int i = -3; i <= 3; i++)
			for (int j = -4; j <= 4; j++)
				TextOut(mdc, fx + i, fy + j, L"L SHIFT", lstrlen(L"L SHIFT"));
		SetTextColor(mdc, RGB(255, 255, 255));
		TextOut(mdc, fx, fy, L"L SHIFT", lstrlen(L"L SHIFT"));
	}

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}