#include <atlImage.h>
#include "images.h"


CImage BackGround, indicator_back, ammo_icon, ammo_lmg_icon, ammo_sniper_icon, ammo_clip_icon, ammo_hbrush_icon;
CImage commando_right, commando_left, commando_jump_right, commando_jump_left;
CImage commando_fire_right, commando_fire_left, commando_jump_fire_right, commando_jump_fire_left, commando_zoom_right, commando_zoom_left;
CImage SCAR_H_right, SCAR_H_left, M16_right, M16_left, MP44_right, MP44_left, MG42_right, MG42_left, AWP_right, AWP_left, hbrush_right, hbrush_left;
CImage m1_right, m1_left;
CImage monster_right, monster_left, monster_big_right, monster_big_left;
CImage monster_air_right[3], monster_air_left[3];
CImage exp_icon, flame_right, flame_left;
CImage zoom_complited, zoom_targeted;
CImage CM_dead;
CImage monster_dead_left, monster_dead_right, monster_big_dead_left, monster_big_dead_right, monster_air_dead;
CImage catridge[4];
CImage BG_paused, CM_paused;
CImage grenade[8];
CImage avail_grenade, unavail_grenade;
CImage explode[7];
CImage logo, background_main, intro_logo, CM_game_start, background_game_start, logo_game_start;
CImage clip[4], CM_loading[2];

gun_catridge gc[2000];

//메인 화면 백그라운드 스크롤
int Scanner_main;
//메인 로고 y좌표
int logo_y = 170;
//메인 로고 애니메이션 가속
int logo_acc = 30;
//아머리에서 메인으로 나갈때의 로고 애니메이션 재생
BOOL armory_to_main = FALSE;
//메인에서 아머리로 들어갈때의 로고 애니메이션 재생
BOOL main_to_armory = FALSE;

//새 게임 애니메이션 관련 변수들
int new_bg_x = 0;
int new_bg_y = 850;
int new_acc = 41;
int CM_game_start_x = -800;
int new_logo_y = 850;
int new_logo_acc = 35;
int new_logo_delay = 0;
int new_logo_move = 1;



int cdx = 0; // 탄피 인덱스 (catridge-index)
int cat_delete_delay; //탄피 삭제 딜레이

//일시정지 화면에 쓰이는 가속값과 좌표값
int pause_y = 900;
int pause_acc;
int CM_paused_y = 850;
int cm_pause_acc;

//수류탄 프레임
int g_frame = 0;
//수류탄 위치
int gren_x, gren_y = 0;
int gren_dir = 0;
int gren_motion_dir = 0; //1 up 2 down
int gren_acc = 0;
//폭발 여부
BOOL is_boom = FALSE;
BOOL is_throw = FALSE;
BOOL set_grenade = FALSE;
BOOL able_grenade = TRUE;
int boom_delay = 0;
int gren_delay = 0;
int gren_time = 0;

//폭발 화염 프레임
int ex_frame = 0;
int ex_frame_delay = 0;

//m1 게런드 클립 변수
int clip_x;
int clip_y;
int clip_acc;
int clip_dir;
int clip_motion_dir; //1 up 2 down
int clip_frame;
BOOL clip_created = FALSE;

//_right: 우측 이미지, _left: 좌측 이미지

//배경 이미지 관련 (background)
int BG_w, BG_h; //배경이미지의 가로, 세로 사이즈

//플레이어 이미지 관련 (commando)
int CM_img_dir;   //(0: 좌측, 1: 우측), 초기값은 0(좌측), 무기 이미지 방향도 이 변수를 사용함
int CM_w, CM_h;   //플레이어 캐릭터 이미지의 가로, 세로 사이즈

//몬스터 관련
int MST_w, MST_h; //몬스터 이미지의 사이즈
int MST_big_w, MST_big_h; //대형 몬스터 이미지의 사이즈
int MST_air_w, MST_air_h; //공중 몬스터 이미지의 사이즈

int Fdelay_air; // 타이머 속도가 빨라 프레임 속도를 늧추는 수단이 필요
int air; //공중 몬스터 애니메이션 인덱스
int up_down = 1; //공중 몬스터 애니메이션의 인덱스 감소 및 증가 결정, 기본값 1, 증가


int GUN_number = 1;    //무기 번호
int GUN_w, GUN_h;      //무기(총) 이미지의 가로, 세로 사이즈
//무기의 위치 및 방향은 플레이어를 기준으로 하기 때문에 별도의 좌표 변수는 없음

int BG_scanner = 1500; //배경 읽기 시작하는 좌표, 수치가 클 수록 배경은 오른쪽으로 이동한다. 1500px씩만 보여준다.
int landing_shake;     //플레이어 착지 시 흔드림 표현, 모든 오브젝트의 y좌표에 더한다.
int shake_reduce;      //1이면 착지 후 화면이 복구, 0이면 끝
int is_land;           //착지 여부 검사

int IND_w, IND_h;	  //장탄수 표시기 배경 이미지 크기
int AMO_w, AMO_h;	  //장탄수 표시기에 표시되는 아이콘 이미지 크기

int ss_x;			  //사격 시 화면 흔들림, 모든 오브젝트의 x좌표에 더한다. //(shoot shake_x)
int ss_y;			  //사격 시 화면 흔들림, 모든 오브젝트의 y좌표에 더한다. //(shoot_shake_y)
int shake_count;		  //흔들리는 횟수를 센다
int ind_effect;		  //1이면 활성화, 0이면 비활성화
int shake_effect;	  //''
int shake_acc;		  //부드러운 흔들림 효과를 위해 감속 적용

int ypos_zc = 0; //사격 가능 표시의 y좌표, 수치가 커질수록 플레이어 머리 위로 더 높이 올라간다.
int ypos_zc_acc = 15; //사격 가능 표시의 애니메이션 가속도

int death_x = -500;
int death_acc = 54;

//탄피 인덱스 밀어내기
void push_cat(int idx) {
	for (int i = 0; i < idx - 1; i++) {
		gc[i].x = gc[i + 1].x;
		gc[i].y = gc[i + 1].y;
		gc[i].dir = gc[i + 1].dir;
		gc[i].motion_dir = gc[i + 1].motion_dir;
		gc[i].frame = gc[i + 1].frame;
		gc[i].x_speed = gc[i + 1].x_speed;
		gc[i].acc = gc[i + 1].acc;
	}
}

void ellipse_intro(HDC mdc, RECT rt, int size, double r, double g, double b) {
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	hbrush = CreateSolidBrush(RGB(r, g, b));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);
	hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	Ellipse(mdc, rt.right / 2 - size, rt.bottom / 2 - size, rt.right / 2 + size, rt.bottom / 2 + size);

	SelectObject(mdc, oldbrush);
	DeleteObject(hbrush);
	SelectObject(mdc, oldpen);
	DeleteObject(hpen);

}

void ellipse_intro2(HDC mdc, RECT rt, int size) {
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	Ellipse(mdc, rt.right / 2 - size, rt.bottom / 2 - size, rt.right / 2 + size, rt.bottom / 2 + size);


	hbrush = CreateSolidBrush(RGB(255, 255, 255));
	oldbrush = (HBRUSH)SelectObject(mdc, hbrush);
	hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	Ellipse(mdc, rt.right / 2 - (size - 20), rt.bottom / 2 - (size - 20), rt.right / 2 + (size - 20), rt.bottom / 2 + (size - 20));

	SelectObject(mdc, oldbrush);
	DeleteObject(hbrush);
	SelectObject(mdc, oldpen);
	DeleteObject(hpen);

}

void press_space(HDC mdc, int y) {
	HFONT hfont, oldfont;
	hfont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(mdc, hfont);
	SetBkMode(mdc, TRANSPARENT);

	for (int i = -3; i <= 3; i++)
		for (int j = -4; j <= 4; j++)
			TextOut(mdc, 600 + i, y + j, L"- Press Space -", lstrlen(L"- Press Space -"));

	SetTextColor(mdc, RGB(255, 255, 255));
	TextOut(mdc, 600, y, L"- Press Space -", lstrlen(L"- Press Space -"));

	SelectObject(mdc, oldfont);
	DeleteObject(hfont);
}