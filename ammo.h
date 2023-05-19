#pragma once
struct ammo_pos { //�Ѿ˵��� ��ġ, ����, �����̴� ���θ� �����ϴ� ����ü
	double x, y;
	double x2, y2;
	double angle;
	int is_shoot; //0�̸� ����, 1�̸� ������
};

static int apx; //���ư��� �Ѿ˵��� ��ġ���� �����ϴ� ����ü�� �ε���(ammo-pos-index)

static int shoot_delay; //���� �ӵ� ���� ���� //��ġ�� �������� ���簡 ������
static int r_down; //������ �ߺ� �Է� ������ ���� ����
static int reload; //0�̸� ������ ����, 1�̸� ������ ��

static int ind_size = 100; //ammo_indicator�� ��Ʈ ũ��
static int ind_x = 1320; //ammo_indicator�� ��Ʈ ��ġ
static int ind_y = 650; //ammo_indicator�� ��Ʈ ��ġ

static int ind_ani; //�Ѿ��� �Ҹ� �� ������ �ִϸ��̼� ���, 0: ��� ����, 1: ���

static int reload_x; //������ ǥ�ñ� x2��ǥ

void make_ammo(int apx, double CM_x, double CM_y, double my, double mx, double var); //�Ѿ� ��ü�� ����� �Լ�
void draw_ammo(HDC mdc, double x, double y, double x2, double y2); //�Ѿ��� �׸��� �Լ�
void ammo_move(int apx, int is_shoot, RECT rt);
void ammo_indicator(HDC mdc, int apx, int GUN_number, int size, int ind_x, int ind_y);
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4);