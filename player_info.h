#pragma once
extern int space_pressed;       //���� Ű �ߺ��� �����ϱ� ���� ����
extern int CM_move_dir; // 0: �����̵�, 1: ���� �̵� -1: ����
extern int CM_jump_acc; //���� �� ���ӵ�
extern int CM_jump;          //0: ���� �� ��, 1: ���� �� 2: ���� ��

extern double CM_x, CM_y;    //�÷��̾� ĳ������ ��ġ

extern bool is_zoom;
extern bool avail_awp;

extern int health;
extern int recovery_delay;

void player_health(HDC mdc, RECT rt, double ss_x, double ss_y, double landing_shake, int health);