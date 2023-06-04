#include <atlImage.h>


CImage BackGround, indicator_back, ammo_icon, ammo_lmg_icon, ammo_sniper_icon;
CImage commando_right, commando_left, commando_jump_right, commando_jump_left;
CImage commando_fire_right, commando_fire_left, commando_jump_fire_right, commando_jump_fire_left, commando_zoom_right, commando_zoom_left;
CImage SCAR_H_right, SCAR_H_left, M16_right, M16_left, MP44_right, MP44_left, MG42_right, MG42_left, AWP_right, AWP_left;
CImage monster_right, monster_left, monster_big_right, monster_big_left;
CImage monster_air_right[3], monster_air_left[3];
CImage exp_icon, flame_right, flame_left;
CImage zoom_complited, zoom_targeted;

//������ ���� �׽�Ʈ�� �̹�����, �Ϸ� ������ �� �� �ٵ��� �̹����� ��ü ����
//_right: ���� �̹���, _left: ���� �̹���

//��� �̹��� ���� (background)
int BG_w, BG_h; //����̹����� ����, ���� ������

//�÷��̾� �̹��� ���� (commando)
int CM_img_dir;   //(0: ����, 1: ����), �ʱⰪ�� 0(����), ���� �̹��� ���⵵ �� ������ �����
int CM_w, CM_h;   //�÷��̾� ĳ���� �̹����� ����, ���� ������

//���� ����
int MST_w, MST_h; //���� �̹����� ������
int MST_big_w, MST_big_h; //���� ���� �̹����� ������
int MST_air_w, MST_air_h; //���� ���� �̹����� ������

int Fdelay_air; // Ÿ�̸� �ӵ��� ���� ������ �ӵ��� �n�ߴ� ������ �ʿ�
int air; //���� ���� �ִϸ��̼� �ε���
int up_down = 1; //���� ���� �ִϸ��̼��� �ε��� ���� �� ���� ����, �⺻�� 1, ����


int GUN_number = 1;    //���� ��ȣ
int GUN_w, GUN_h;      //����(��) �̹����� ����, ���� ������
//������ ��ġ �� ������ �÷��̾ �������� �ϱ� ������ ������ ��ǥ ������ ����

int BG_scanner = 1500; //��� �б� �����ϴ� ��ǥ, ��ġ�� Ŭ ���� ����� ���������� �̵��Ѵ�. 1500px���� �����ش�.
int landing_shake;     //�÷��̾� ���� �� ��帲 ǥ��, ��� ������Ʈ�� y��ǥ�� ���Ѵ�.
int shake_reduce;      //1�̸� ���� �� ȭ���� ����, 0�̸� ��
int is_land;           //���� ���� �˻�

int IND_w, IND_h;	  //��ź�� ǥ�ñ� ��� �̹��� ũ��
int AMO_w, AMO_h;	  //��ź�� ǥ�ñ⿡ ǥ�õǴ� ������ �̹��� ũ��

int ss_x;			  //��� �� ȭ�� ��鸲, ��� ������Ʈ�� x��ǥ�� ���Ѵ�. //(shoot shake_x)
int ss_y;			  //��� �� ȭ�� ��鸲, ��� ������Ʈ�� y��ǥ�� ���Ѵ�. //(shoot_shake_y)
int shake_count;		  //��鸮�� Ƚ���� ����
int ind_effect;		  //1�̸� Ȱ��ȭ, 0�̸� ��Ȱ��ȭ
int shake_effect;	  //''
int shake_acc;		  //�ε巯�� ��鸲 ȿ���� ���� ���� ����

int ypos_zc = 0; //��� ���� ǥ���� y��ǥ, ��ġ�� Ŀ������ �÷��̾� �Ӹ� ���� �� ���� �ö󰣴�.
int ypos_zc_acc = 15; //��� ���� ǥ���� �ִϸ��̼� ���ӵ�