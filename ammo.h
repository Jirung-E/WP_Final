#pragma once
struct ammo_pos { //총알들의 위치, 각도, 움직이는 여부를 저장하는 구조체
	double x, y;
	double x2, y2;
	double angle;
	int is_shoot; //0이면 정지, 1이면 움직임
};

static int apx; //날아가는 총알들의 위치들을 저장하는 구조체의 인덱스(ammo-pos-index)

static int shoot_delay; //연사 속도 조절 변수 //수치가 높을수록 연사가 느려짐
static int r_down; //재장전 중복 입력 방지를 위한 변수
static int reload; //0이면 재장전 안함, 1이면 재장전 중

static int ind_size = 100; //ammo_indicator의 폰트 크기
static int ind_x = 1320; //ammo_indicator의 폰트 위치
static int ind_y = 650; //ammo_indicator의 폰트 위치

static int ind_ani; //총알이 소모 될 때마다 애니메이션 재생, 0: 재생 안함, 1: 재생

static int reload_x; //재장전 표시기 x2좌표

void make_ammo(int apx, double CM_x, double CM_y, double my, double mx, double var); //총알 객체를 만드는 함수
void draw_ammo(HDC mdc, double x, double y, double x2, double y2); //총알을 그리는 함수
void ammo_move(int apx, int is_shoot, RECT rt);
void ammo_indicator(HDC mdc, int apx, int GUN_number, int size, int ind_x, int ind_y);
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4);