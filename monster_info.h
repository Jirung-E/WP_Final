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

static int mdx_r;                 //�Ϲ� ���� �� (monster - index_regular)
static int spawn_timer_r = 300;   //���尡 �ö� ���� ���� �ֱⰡ ��������. -> ��ġ�� ��������. �⺻ ��ġ�� 300���� ����

static int mdx_big;				  //���� ���� ��
static int spawn_timer_big = 800;

void spawn_monster_regular(int mdx, int BG_scanner, RECT rt);
void spawn_monster_big(int mdx_big, int BG_scanner, RECT rt);

void update_monster_dir_r(double monster_x, double CM_x, int mdx);
void update_monster_dir_big(double monster_x, double CM_x, int mdx_big);

void monster_hp_ind(HDC mdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
