#pragma once
static BOOL is_draw = FALSE;					  //TRUE�� �� �Ѿ� ������ �׸�
static int ammo;							      //��ź ��
static int draw_timer;							  //������ �׷����� �ð��� ���Ѵ�. ��ġ�� �������� �� �������� �׷�����.
static double angle;							  //�Ѿ� ������ ����
static double ammo_x1, ammo_y1, ammo_x2, ammo_y2; //�Ѿ� ������ �׷����� ��ǥ
static double dist;//�÷��̾�� ��Ʈ ������ ������ �Ÿ�

static BOOL draw_hit = FALSE;					  //TRUE�� �� ��Ʈ ����Ʈ�� �׸�
static int hit_timer;							  //��Ʈ ����Ʈ�� �׷����� �ð�	
static int hit;									  //��Ʈ ������ ���� ������ �ε���
static double hit_x, hit_y;   //������ ���ο��� �����Ǵ� ���� x,y ��ǥ

static double var;            //���� ���� ����� ���� �ݵ����� ���� ��Ȯ���� ������, ��ġ�� ������ ���� �л굵�� Ŀ��

static int ind_size = 100; //ammo_indicator�� ��Ʈ ũ��
static int ind_x = 1360;   //ammo_indicator�� ��Ʈ ��ġ
static int ind_y = 650;    //ammo_indicator�� ��Ʈ ��ġ
static int ind_ani;        //�Ѿ��� �Ҹ� �� ������ �ִϸ��̼� ���, 0: ��� ����, 1: ���

static int shoot_delay; //���� �ӵ� ���� ���� //��ġ�� �������� ���簡 ������
static int r_pressed;      //������ �ߺ� �Է� ������ ���� ����
static int reload;      //0�̸� ������ ����, 1�̸� ������ ��
static int reload_x;    //������ ǥ�ñ� x2 ��ǥ

static int empty;   //�Ѿ� ��� �Ҹ� �� 1�� ����

static BOOL can_shoot = TRUE; //���� �ӵ��� ���� ���� ���콺 ��Ÿ�Ͽ� �� ������ ��� ���� ���� ����
static int after_delay;       //�� ������ ���� �ӵ� ��ġ�� �ٸ��� ���콺�� �ƹ��� ���� ������ �߻簡 ���� �ʴ´�.

static int reload_delay; //mg_42 ������ �ӵ� ���� ����

int cal_dist(double x1, double y1, double x2, double y2);
void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2);
void ammo_indicator(HDC mdc, int max_ammo, int ammo, int ind_size, int ind_x, int ind_y);
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4);
int cal_damage(int monster_hp, int GUN_number);


