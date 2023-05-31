#pragma once
//경험치. exp로 변수 이름을 하려 했으나 충돌로 인해 이렇게 함
static int experience;
//경험치가 증가하는 정도를 저장하는 변수. 경험치 증가 효과를 출력할 때 사용한다.
static int prev_up;
//경험치가 증가할 때 경험치 증가 이벤트를 발생시킨다.
static BOOL exp_up = FALSE;
static int exp_x = -150;
static int exp_acc = 20;
static int out = 1;
static int exp_out_delay;

void show_exp(HDC mdc, int experience, int x, int y);
void show_exp_add(HDC mdc, int prev_up, int x, int y);