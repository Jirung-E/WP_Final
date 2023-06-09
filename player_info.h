#pragma once
extern int space_pressed;       //점프 키 중복을 방지하기 위한 변수
extern int CM_move_dir; // 0: 좌측이동, 1: 우측 이동 -1: 정지
extern int CM_jump_acc; //점프 시 가속도
extern int CM_jump;          //0: 점프 안 함, 1: 점프 함 2: 착지 중

extern double CM_x, CM_y;    //플레이어 캐릭터의 위치

extern bool is_zoom;
extern bool avail_awp;

//awp는 볼트를 당겨야 탄피가 배출되므로 딜레이를 주어 탄피를 생성한다.
//오류가 나서 BOOL 대신에 int로 대체
extern int cat_ready; 
extern int awp_cat_delay;

extern int health;
extern int recovery_delay;

//대미지 쿨타임
extern int cool_time;

void player_health(HDC mdc, RECT rt, double ss_x, double ss_y, double landing_shake, int health);
void show_gren_time(HDC mdc, double ss_x, double ss_y, double landing_shake, int gren_time);