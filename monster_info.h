#pragma once
struct monster_info_regular { //������ ���� ����, �̵� ����, ��ġ�� �����ϴ� ����ü
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
}; 

struct monster_info_big { //���� ����
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
};

struct monster_info_air { //���� ����
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
};

static int mdx_r = 0;                 //�Ϲ� ���� �� (monster - index_regular)
static int spawn_timer_r = 500;   //���尡 �ö� ���� ���� �ֱⰡ ��������. -> ��ġ�� ��������. �⺻ ��ġ�� 500���� ����

static int mdx_big = 0;				  //���� ���� ��
static int spawn_timer_big = 1000;

static int mdx_air = 0;				  //���� ���� ��
static int spawn_timer_air = 600;

void spawn_monster_regular(int mdx, int BG_scanner, RECT rt);
void spawn_monster_big(int mdx_big, int BG_scanner, RECT rt);
void spawn_monster_air(int mdx_air, int BG_scanner, RECT rt);

void update_monster_dir_r(double monster_x, double CM_x, int mdx);
void update_monster_dir_big(double monster_x, double CM_x, int mdx_big);
void update_monster_dir_air(double monster_x, double CM_x, int mdx_air);

void monster_hp_ind(HDC mdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

void monster_array_push_r(int hit, int idx);
void monster_array_push_big(int hit, int idx);
void monster_array_push_air(int i, int mdx_air);