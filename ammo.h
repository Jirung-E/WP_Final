#pragma once
struct ellipse {
	int size;
	double x, y;
	int dir;
	int acc;
};

extern int edx; //ellipse-index
extern double incline;
extern double rect_x_end, rect_y_end;
extern double rect_x_start, rect_y_start;
extern int rect_size_x, rect_size_y;
extern int rect_dir;
extern BOOL is_lazer;
extern BOOL can_make_rect;
extern BOOL is_rect;
extern int rect_acc;
extern int rect_delay;
extern int rect_delete_delay;


extern BOOL is_draw;					  //TRUE일 시 총알 궤적을 그림
extern int ammo;							      //장탄 수
extern int draw_timer;							  //궤적이 그려지는 시간을 정한다. 수치가 높을수록 더 오랫동안 그려진다.
extern double angle;							  //총알 궤적의 각도
extern double ammo_x1, ammo_y1, ammo_x2, ammo_y2; //총알 궤적이 그려지는 좌표
extern double dist;//플레이어와 히트 판정점 사이의 거리

extern BOOL draw_hit;					  //TRUE일 시 히트 포인트를 그림
extern int hit_timer;							  //히트 포인트가 그려지는 시간	
extern int hit;									  //히트 판정을 받은 몬스터의 인덱스
extern double hit_x, hit_y;   //조준점 내부에서 생성되는 실제 x,y 좌표

extern double var;            //총을 오래 사격할 수록 반동으로 인해 정확도가 떨어짐, 수치가 증가할 수록 분산도가 커짐

extern int ind_size; //ammo_indicator의 폰트 크기
extern int ind_x;   //ammo_indicator의 폰트 위치
extern int ind_y;    //ammo_indicator의 폰트 위치
extern int ind_ani;        //총알이 소모 될 때마다 애니메이션 재생, 0: 재생 안함, 1: 재생

extern int shoot_delay; //연사 속도 조절 변수 //수치가 높을수록 연사가 느려짐
extern int r_pressed;      //재장전 중복 입력 방지를 위한 변수
extern int reload;      //0이면 재장전 안함, 1이면 재장전 중
extern int reload_x;    //재장전 표시기 x2 좌표

extern int empty;   //총알 모두 소모 시 1로 변경

extern BOOL can_shoot; //연사 속도가 느린 총을 마우스 연타하여 더 빠르게 쏘는 부정 행위 방지
extern int after_delay;       //이 변수가 연사 속도 수치와 다르면 마우스를 아무리 빨리 눌러도 발사가 되지 않는다.

extern int reload_delay; //mg_42 재장전 속도 조정 변수

int cal_dist(double x1, double y1, double x2, double y2);
void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2, int GUN_number);
void ammo_indicator(HDC mdc, int max_ammo, int ammo, int ind_size, int ind_x, int ind_y);
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4);
int cal_damage(int monster_hp, int GUN_number);
void push_ellipse(int del, int idx);


