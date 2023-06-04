#pragma once
extern BOOL is_draw;					  //TRUE�� �� �Ѿ� ������ �׸�
extern int ammo;							      //��ź ��
extern int draw_timer;							  //������ �׷����� �ð��� ���Ѵ�. ��ġ�� �������� �� �������� �׷�����.
extern double angle;							  //�Ѿ� ������ ����
extern double ammo_x1, ammo_y1, ammo_x2, ammo_y2; //�Ѿ� ������ �׷����� ��ǥ
extern double dist;//�÷��̾�� ��Ʈ ������ ������ �Ÿ�

extern BOOL draw_hit;					  //TRUE�� �� ��Ʈ ����Ʈ�� �׸�
extern int hit_timer;							  //��Ʈ ����Ʈ�� �׷����� �ð�	
extern int hit;									  //��Ʈ ������ ���� ������ �ε���
extern double hit_x, hit_y;   //������ ���ο��� �����Ǵ� ���� x,y ��ǥ

extern double var;            //���� ���� ����� ���� �ݵ����� ���� ��Ȯ���� ������, ��ġ�� ������ ���� �л굵�� Ŀ��

extern int ind_size; //ammo_indicator�� ��Ʈ ũ��
extern int ind_x;   //ammo_indicator�� ��Ʈ ��ġ
extern int ind_y;    //ammo_indicator�� ��Ʈ ��ġ
extern int ind_ani;        //�Ѿ��� �Ҹ� �� ������ �ִϸ��̼� ���, 0: ��� ����, 1: ���

extern int shoot_delay; //���� �ӵ� ���� ���� //��ġ�� �������� ���簡 ������
extern int r_pressed;      //������ �ߺ� �Է� ������ ���� ����
extern int reload;      //0�̸� ������ ����, 1�̸� ������ ��
extern int reload_x;    //������ ǥ�ñ� x2 ��ǥ

extern int empty;   //�Ѿ� ��� �Ҹ� �� 1�� ����

extern BOOL can_shoot; //���� �ӵ��� ���� ���� ���콺 ��Ÿ�Ͽ� �� ������ ��� ���� ���� ����
extern int after_delay;       //�� ������ ���� �ӵ� ��ġ�� �ٸ��� ���콺�� �ƹ��� ���� ������ �߻簡 ���� �ʴ´�.

extern int reload_delay; //mg_42 ������ �ӵ� ���� ����

int cal_dist(double x1, double y1, double x2, double y2);
void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2, int GUN_number);
void ammo_indicator(HDC mdc, int max_ammo, int ammo, int ind_size, int ind_x, int ind_y);
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4);
int cal_damage(int monster_hp, int GUN_number);


