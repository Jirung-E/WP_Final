#pragma once
static CImage commando_right, commando_left, SCAR_H_right, SCAR_H_left, BackGround, indicator_back, ammo_icon;
//������ ���� �׽�Ʈ�� �̹�����, �Ϸ� ������ �� �� �ٵ��� �̹����� ��ü ����
//_right: ���� �̹���, _left: ���� �̹���
//backGround: ���ۿ� �̹�����, ����� ������ ����. ������ �׽�Ʈ�� �� �������, ���� �ٸ� �̹����� ��ü ����

//��� �̹��� ���� (background)
static int BG_w, BG_h; //����̹����� ����, ���� ������

//�÷��̾� �̹��� ���� (commando)
static int CM_img_dir; //(0: ����, 1: ����), �ʱⰪ�� 0(����), ���� �̹��� ���⵵ �� ������ �����
static int CM_w, CM_h; //�÷��̾� ĳ���� �̹����� ����, ���� ������

//���� �̹��� ���� (gun)
static int GUN_number = 1; //���� ��ȣ
static int GUN_w, GUN_h; //����(��) �̹����� ����, ���� ������
//������ ��ġ �� ������ �÷��̾ �������� �ϱ� ������ ������ ��ǥ ������ ����

static int BG_scanner; //��� �б� �����ϴ� ��ǥ, ��ġ�� Ŭ ���� ����� ���������� �̵��Ѵ�.
static int landing_shake; //�÷��̾� ���� �� ��帲 ǥ��, ��� ������Ʈ�� y��ǥ�� ���Ѵ�.
static int shake_reduce; //1�̸� ���� �� ȭ���� ����, 0�̸� ��
static int is_land; //���� ���� �˻�

static int IND_w, IND_h; //��ź�� ǥ�ñ� ��� �̹��� ũ��
static int AMO_w, AMO_h; //��ź�� ǥ�ñ⿡ ǥ�õǴ� ������ �̹��� ũ��

static int ss_x; //��� �� ȭ�� ��鸲, ��� ������Ʈ�� x��ǥ�� ���Ѵ�. //(shoot shake_x)
static int ss_y; //��� �� ȭ�� ��鸲, ��� ������Ʈ�� y��ǥ�� ���Ѵ�. //(shoot_shake_y)
static int shake_count; //��鸮�� Ƚ���� ����
static int ind_effect;  //1�̸� Ȱ��ȭ, 0�̸� ��Ȱ��ȭ
static int shake_effect;