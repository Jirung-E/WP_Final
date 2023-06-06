#pragma once
extern int space_pressed;       //점프 키 중복을 방지하기 위한 변수
extern int CM_move_dir; // 0: 좌측이동, 1: 우측 이동 -1: 정지
extern int CM_jump_acc; //점프 시 가속도
extern int CM_jump;          //0: 점프 안 함, 1: 점프 함 2: 착지 중

extern double CM_x, CM_y;    //플레이어 캐릭터의 위치

extern bool is_zoom;
extern bool avail_awp;

extern int health;
extern int recovery_delay;

void player_health(HDC mdc, RECT rt, double ss_x, double ss_y, double landing_shake, int health);