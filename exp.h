#pragma once
//����ġ. exp�� ���� �̸��� �Ϸ� ������ �浹�� ���� �̷��� ��
static int experience;
//����ġ�� �����ϴ� ������ �����ϴ� ����. ����ġ ���� ȿ���� ����� �� ����Ѵ�.
static int prev_up;
//����ġ�� ������ �� ����ġ ���� �̺�Ʈ�� �߻���Ų��.
static BOOL exp_up = FALSE;
static int exp_x = -150;
static int exp_acc = 20;
static int out = 1;
static int exp_out_delay;

void show_exp(HDC mdc, int experience, int x, int y);
void show_exp_add(HDC mdc, int prev_up, int x, int y);