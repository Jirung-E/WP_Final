#pragma once
static BOOL is_draw = FALSE;					  //TRUE일 시 총알 궤적을 그림
static int ammo;							      //장탄 수
static int draw_timer;							  //궤적이 그려지는 시간을 정한다. 수치가 높을수록 더 오랫동안 그려진다.
static double angle;							  //총알 궤적의 각도
static double ammo_x1, ammo_y1, ammo_x2, ammo_y2; //총알 궤적이 그려지는 좌표
static double dist;//플레이어와 히트 판정점 사이의 거리

static BOOL draw_hit = FALSE;					  //TRUE일 시 히트 포인트를 그림
static int hit_timer;							  //히트 포인트가 그려지는 시간	
static int hit;									  //히트 판정을 받은 몬스터의 인덱스
static double hit_x, hit_y;   //조준점 내부에서 생성되는 실제 x,y 좌표

static double var;            //총을 오래 사격할 수록 반동으로 인해 정확도가 떨어짐, 수치가 증가할 수록 분산도가 커짐

static int ind_size = 100; //ammo_indicator의 폰트 크기
static int ind_x = 1360;   //ammo_indicator의 폰트 위치
static int ind_y = 650;    //ammo_indicator의 폰트 위치
static int ind_ani;        //총알이 소모 될 때마다 애니메이션 재생, 0: 재생 안함, 1: 재생

static int shoot_delay; //연사 속도 조절 변수 //수치가 높을수록 연사가 느려짐
static int r_pressed;      //재장전 중복 입력 방지를 위한 변수
static int reload;      //0이면 재장전 안함, 1이면 재장전 중
static int reload_x;    //재장전 표시기 x2 좌표

static int empty;   //총알 모두 소모 시 1로 변경

int cal_dist(double x1, double y1, double x2, double y2);
void draw_ammo(HDC mdc, double x1, double y1, double x2, double y2);
void ammo_indicator(HDC mdc, int apx, int GUN_number, int size, int ind_x, int ind_y);
void reload_indicator(HDC mdc, int x, int y, int x2, int y2, int x3, int y3, int x4, int y4);
int cal_damage(int monster_hp, int GUN_number);


