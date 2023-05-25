#pragma once
struct monster_info_regular { //몬스터의 보는 방향, 이동 방향, 위치를 저장하는 구조체
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
}; 

struct monster_info_big { //대형 몬스터
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
};

static int mdx_r;                 //일반 몬스터 수 (monster - index_regular)
static int spawn_timer_r = 300;   //라운드가 올라갈 수록 스폰 주기가 빨라진다. -> 수치가 낮아진다. 기본 수치는 300부터 시작

static int mdx_big;				  //대형 몬스터 수
static int spawn_timer_big = 800;

void spawn_monster_regular(int mdx, int BG_scanner, RECT rt);
void spawn_monster_big(int mdx_big, int BG_scanner, RECT rt);

void update_monster_dir_r(double monster_x, double CM_x, int mdx);
void update_monster_dir_big(double monster_x, double CM_x, int mdx_big);

void monster_hp_ind(HDC mdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
