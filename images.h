#pragma once
static CImage BackGround, indicator_back, ammo_icon;
static CImage commando_right, commando_left, commando_jump_right, commando_jump_left;
static CImage SCAR_H_right, SCAR_H_left, M16_right, M16_left, MP44_right, MP44_left, MG42_right, MG42_left, AWP_right, AWP_left;
static CImage monster_right, monster_left, monster_big_right, monster_big_left;

//������ ���� �׽�Ʈ�� �̹�����, �Ϸ� ������ �� �� �ٵ��� �̹����� ��ü ����
//_right: ���� �̹���, _left: ���� �̹���

//��� �̹��� ���� (background)
static int BG_w, BG_h; //����̹����� ����, ���� ������

//�÷��̾� �̹��� ���� (commando)
static int CM_img_dir;   //(0: ����, 1: ����), �ʱⰪ�� 0(����), ���� �̹��� ���⵵ �� ������ �����
static int CM_w, CM_h;   //�÷��̾� ĳ���� �̹����� ����, ���� ������

//���� ����
static int MST_w, MST_h; //���� �̹����� ������
static int MST_big_w, MST_big_h; //���� ���� �̹����� ������

//���� �̹��� ���� (gun)
enum gun_name_images {
	null, scar_h, m16, mp_44, mg_42, awp
};

static int GUN_number = 1;    //���� ��ȣ
static int GUN_w, GUN_h;      //����(��) �̹����� ����, ���� ������
//������ ��ġ �� ������ �÷��̾ �������� �ϱ� ������ ������ ��ǥ ������ ����

static int BG_scanner = 1500; //��� �б� �����ϴ� ��ǥ, ��ġ�� Ŭ ���� ����� ���������� �̵��Ѵ�. 1500px���� �����ش�.
static int landing_shake;     //�÷��̾� ���� �� ��帲 ǥ��, ��� ������Ʈ�� y��ǥ�� ���Ѵ�.
static int shake_reduce;      //1�̸� ���� �� ȭ���� ����, 0�̸� ��
static int is_land;           //���� ���� �˻�

static int IND_w, IND_h;	  //��ź�� ǥ�ñ� ��� �̹��� ũ��
static int AMO_w, AMO_h;	  //��ź�� ǥ�ñ⿡ ǥ�õǴ� ������ �̹��� ũ��

static int ss_x;			  //��� �� ȭ�� ��鸲, ��� ������Ʈ�� x��ǥ�� ���Ѵ�. //(shoot shake_x)
static int ss_y;			  //��� �� ȭ�� ��鸲, ��� ������Ʈ�� y��ǥ�� ���Ѵ�. //(shoot_shake_y)
static int shake_count;		  //��鸮�� Ƚ���� ����
static int ind_effect;		  //1�̸� Ȱ��ȭ, 0�̸� ��Ȱ��ȭ
static int shake_effect;	  //''
static int shake_acc;		  //�ε巯�� ��鸲 ȿ���� ���� ���� ����