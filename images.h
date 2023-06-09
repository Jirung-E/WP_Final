#pragma once

#include <atlimage.h>

extern CImage BackGround, indicator_back, ammo_icon, ammo_lmg_icon, ammo_sniper_icon;
extern CImage commando_right, commando_left, commando_jump_right, commando_jump_left;
extern CImage commando_fire_right, commando_fire_left, commando_jump_fire_right, commando_jump_fire_left, commando_zoom_right, commando_zoom_left;
extern CImage SCAR_H_right, SCAR_H_left, M16_right, M16_left, MP44_right, MP44_left, MG42_right, MG42_left, AWP_right, AWP_left;
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
extern CImage logo, background_main;

//���� ��׶��� ��ũ��
extern int Scanner_main;

struct gun_catridge {
    double x, y;
    int frame;
    int acc;
    int x_speed;
    int motion_dir;
    int dir;
};

extern int cdx; // ź�� �ε��� (catridge-index)
extern int cat_delete_delay;

//�Ͻ����� ȭ�鿡 ���̴� ���Ӱ��� ��ǥ��
extern int CM_paused_y;
extern int pause_y;
extern int pause_acc;
extern int cm_pause_acc;

//����ź ������
extern int g_frame;
//����ź ��ġ
extern int gren_x, gren_y;
extern int gren_dir;
extern int gren_motion_dir; //1 up 2 down
extern int gren_acc;

//���� ����
extern BOOL is_boom;
extern BOOL is_throw;
extern BOOL set_grenade;
extern BOOL able_grenade;
extern int boom_delay;
extern int gren_delay;
extern int gren_time;

extern int ex_frame;
extern int ex_frame_delay;

//������ ���� �׽�Ʈ�� �̹�����, �Ϸ� ������ �� �� �ٵ��� �̹����� ��ü ����
//_right: ���� �̹���, _left: ���� �̹���

//��� �̹��� ���� (background)
extern int BG_w, BG_h; //����̹����� ����, ���� ������

//�÷��̾� �̹��� ���� (commando)
extern int CM_img_dir;   //(0: ����, 1: ����), �ʱⰪ�� 0(����), ���� �̹��� ���⵵ �� ������ �����
extern int CM_w, CM_h;   //�÷��̾� ĳ���� �̹����� ����, ���� ������

//���� ����
extern int MST_w, MST_h; //���� �̹����� ������
extern int MST_big_w, MST_big_h; //���� ���� �̹����� ������
extern int MST_air_w, MST_air_h; //���� ���� �̹����� ������

extern int Fdelay_air; // Ÿ�̸� �ӵ��� ���� ������ �ӵ��� �n�ߴ� ������ �ʿ�
extern int air; //���� ���� �ִϸ��̼� �ε���
extern int up_down; //���� ���� �ִϸ��̼��� �ε��� ���� �� ���� ����, �⺻�� 1, ����

//���� �̹��� ���� (gun)
enum gun_name_images {
    null, scar_h, m16, mp_44, mg_42, awp
};

extern int GUN_number;    //���� ��ȣ
extern int GUN_w, GUN_h;      //����(��) �̹����� ����, ���� ������
//������ ��ġ �� ������ �÷��̾ �������� �ϱ� ������ ������ ��ǥ ������ ����

extern int BG_scanner; //��� �б� �����ϴ� ��ǥ, ��ġ�� Ŭ ���� ����� ���������� �̵��Ѵ�. 1500px���� �����ش�.
extern int landing_shake;     //�÷��̾� ���� �� ��帲 ǥ��, ��� ������Ʈ�� y��ǥ�� ���Ѵ�.
extern int shake_reduce;      //1�̸� ���� �� ȭ���� ����, 0�̸� ��
extern int is_land;           //���� ���� �˻�

extern int IND_w, IND_h;	  //��ź�� ǥ�ñ� ��� �̹��� ũ��
extern int AMO_w, AMO_h;	  //��ź�� ǥ�ñ⿡ ǥ�õǴ� ������ �̹��� ũ��

extern int ss_x;			  //��� �� ȭ�� ��鸲, ��� ������Ʈ�� x��ǥ�� ���Ѵ�. //(shoot shake_x)
extern int ss_y;			  //��� �� ȭ�� ��鸲, ��� ������Ʈ�� y��ǥ�� ���Ѵ�. //(shoot_shake_y)
extern int shake_count;		  //��鸮�� Ƚ���� ����
extern int ind_effect;		  //1�̸� Ȱ��ȭ, 0�̸� ��Ȱ��ȭ
extern int shake_effect;	  //''
extern int shake_acc;		  //�ε巯�� ��鸲 ȿ���� ���� ���� ����

extern int ypos_zc;
extern int ypos_zc_acc;

extern int death_x;
extern int death_acc;

//�� ���� ���� �� ��Ʈ�� �ִϸ��̼� ���(����)
extern BOOL is_intro;

void push_cat(int idx);