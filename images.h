#pragma once

#include <atlimage.h>

extern CImage BackGround, indicator_back, ammo_icon, ammo_lmg_icon, ammo_sniper_icon, ammo_clip_icon, ammo_hbrush_icon;
extern CImage commando_right, commando_left, commando_jump_right, commando_jump_left;
extern CImage commando_fire_right, commando_fire_left, commando_jump_fire_right, commando_jump_fire_left, commando_zoom_right, commando_zoom_left;
extern CImage SCAR_H_right, SCAR_H_left, M16_right, M16_left, MP44_right, MP44_left, MG42_right, MG42_left, AWP_right, AWP_left, hbrush_right, hbrush_left;
extern CImage m1_right, m1_left;
extern CImage monster_right, monster_left, monster_big_right, monster_big_left;
extern CImage monster_air_right[3], monster_air_left[3];
extern CImage exp_icon, flame_right, flame_left;
extern CImage zoom_complited, zoom_targeted;
extern CImage CM_dead;
extern CImage monster_dead_left, monster_dead_right, monster_big_dead_left, monster_big_dead_right, monster_air_dead;
extern CImage catridge[4];
extern CImage BG_paused, CM_paused;
extern CImage grenade[8];
extern CImage avail_grenade, unavail_grenade;
extern CImage explode[7];
extern CImage logo, background_main, intro_logo, CM_game_start, background_game_start, logo_game_start;
extern CImage clip[4], CM_loading[2];

//메인 백그라운드 스크롤
extern int Scanner_main;
//메인 로고 y좌표
extern int logo_y;
//메인 로고 애니메이션 가속
extern int logo_acc;
//아머리에서 메인으로 나갈때의 로고 애니메이션 재생
extern BOOL armory_to_main;
//메인에서 아머리로 들어갈때의 로고 애니메이션 재생
extern BOOL main_to_armory;

struct gun_catridge {
    double x, y;
    int frame;
    int acc;
    int x_speed;
    int motion_dir;
    int dir;
};

extern int cdx; // 탄피 인덱스 (catridge-index)
extern int cat_delete_delay;

//일시정지 화면에 쓰이는 가속값과 좌표값
extern int CM_paused_y;
extern int pause_y;
extern int pause_acc;
extern int cm_pause_acc;

//수류탄 프레임
extern int g_frame;
//수류탄 위치
extern int gren_x, gren_y;
extern int gren_dir;
extern int gren_motion_dir; //1 up 2 down
extern int gren_acc;

//폭발 여부
extern BOOL is_boom;
extern BOOL is_throw;
extern BOOL set_grenade;
extern BOOL able_grenade;
extern int boom_delay;
extern int gren_delay;
extern int gren_time;

extern int ex_frame;
extern int ex_frame_delay;

//아직은 개발 테스트용 이미지로, 완료 직전에 좀 더 다듬은 이미지로 교체 예정
//_right: 우측 이미지, _left: 좌측 이미지

//배경 이미지 관련 (background)
extern int BG_w, BG_h; //배경이미지의 가로, 세로 사이즈

//플레이어 이미지 관련 (commando)
extern int CM_img_dir;   //(0: 좌측, 1: 우측), 초기값은 0(좌측), 무기 이미지 방향도 이 변수를 사용함
extern int CM_w, CM_h;   //플레이어 캐릭터 이미지의 가로, 세로 사이즈

//몬스터 관련
extern int MST_w, MST_h; //몬스터 이미지의 사이즈
extern int MST_big_w, MST_big_h; //대형 몬스터 이미지의 사이즈
extern int MST_air_w, MST_air_h; //공중 몬스터 이미지의 사이즈

extern int Fdelay_air; // 타이머 속도가 빨라 프레임 속도를 늧추는 수단이 필요
extern int air; //공중 몬스터 애니메이션 인덱스
extern int up_down; //공중 몬스터 애니메이션의 인덱스 감소 및 증가 결정, 기본값 1, 증가

//무기 이미지 관련 (gun)
enum gun_name_images {
    null, scar_h, m16, mp_44, mg_42, awp, m1, h_brush
};

extern int GUN_number;    //무기 번호
extern int GUN_w, GUN_h;      //무기(총) 이미지의 가로, 세로 사이즈
//무기의 위치 및 방향은 플레이어를 기준으로 하기 때문에 별도의 좌표 변수는 없음

extern int BG_scanner; //배경 읽기 시작하는 좌표, 수치가 클 수록 배경은 오른쪽으로 이동한다. 1500px씩만 보여준다.
extern int landing_shake;     //플레이어 착지 시 흔드림 표현, 모든 오브젝트의 y좌표에 더한다.
extern int shake_reduce;      //1이면 착지 후 화면이 복구, 0이면 끝
extern int is_land;           //착지 여부 검사

extern int IND_w, IND_h;	  //장탄수 표시기 배경 이미지 크기
extern int AMO_w, AMO_h;	  //장탄수 표시기에 표시되는 아이콘 이미지 크기

extern int ss_x;			  //사격 시 화면 흔들림, 모든 오브젝트의 x좌표에 더한다. //(shoot shake_x)
extern int ss_y;			  //사격 시 화면 흔들림, 모든 오브젝트의 y좌표에 더한다. //(shoot_shake_y)
extern int shake_count;		  //흔들리는 횟수를 센다
extern int ind_effect;		  //1이면 활성화, 0이면 비활성화
extern int shake_effect;	  //''
extern int shake_acc;		  //부드러운 흔들림 효과를 위해 감속 적용

extern int ypos_zc;
extern int ypos_zc_acc;

extern int death_x;
extern int death_acc;

static BOOL is_menu = FALSE;
//새 게임 시작 시 인트로 애니메이션 재생(예정)
static BOOL is_intro = FALSE;
//인트로 딜레이 변수
static int intro_delay = 400;
//이 변수가 0이 되어야 메인으로 진입
static int intro_time = 500;
//배경 변수
static int ellipse_size = 0;
static int ellipse2_size = 0;

static double r = 255, g = 255, b = 255;
//팀 로고 변수
static int intro_logo_x = 400;
static int intro_logo_y = 850;
static int intro_logo_acc = 35;

//로딩 화면 애니메이션 프레임
static int loading_frame = 0;
static int loading_delay = 0;

//새 게임 애니메이션 재생 여부, 재생 시간
extern BOOL into_the_game;
static BOOL end_new_game;
static int into_time = 200;

//새 게임 애니메이션 관련 변수들
extern int new_bg_x;
extern int new_bg_y;
extern int new_acc;
extern int CM_game_start_x;
extern int new_logo_y;
extern int new_logo_acc;
extern int new_logo_delay;
extern int new_logo_move; //1: first, 2: second

//m1게런드 탄클립 변수
extern int clip_x;
extern int clip_y;
extern int clip_acc;
extern int clip_dir;
extern int clip_motion_dir; //1 up 2 down
extern int clip_frame;
extern BOOL clip_created;

//press space 애니메이션
static int press_y = 640;
static int press_acc = 15;
static int logo_acc_enter = 15;

void push_cat(int idx);
void ellipse_intro(HDC mdc, RECT rt, int size, double r, double g, double b);
void ellipse_intro2(HDC mdc, RECT rt, int size);
void press_space(HDC mdc, int y);