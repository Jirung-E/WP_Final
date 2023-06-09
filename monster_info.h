#pragma once
struct monster_info_regular { //몬스터의 보는 방향, 이동 방향, 위치를 저장하는 구조체
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
	int targeted;
	int height;
	int motion_acc;
};

struct monster_info_big { //대형 몬스터
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
	int targeted;
	int height;
	int motion_acc;
};

struct monster_info_air { //공중 몬스터
	double x, y;
	int move_dir;
	int img_dir;
	int hp;
	int height;
	int motion_acc;
};

//몬스터가 죽은 후 시체 이미지 출력. 가장 먼저 생성된 이미지부터 사라진다.
struct dead_location {
	int monster_type; //1: 일반 몬스터, 2: 대형 몬스터, 3: 공중 몬스터
	double x, y;
	int acc;
	int dir;
	int motion_dir;
};

//라운드 올라갈 시 라운드 업 애니메이션을 재생하기 위한 변수
extern int round_size;
extern int round_x;
extern int kill_count;
extern BOOL round_up; //이 변수가 TRUE가 되면 애니메이션을 재생한다.

extern int game_round;			// 스폰 주기를 결정함

//몬스터 시체 인덱스
extern int ddx; //dead-index
extern int delete_delay;

extern int mdx_r;                 //일반 몬스터 수 (monster - index_regular)
extern int spawn_timer_r;   //라운드가 올라갈 수록 스폰 주기가 빨라진다. -> 수치가 낮아진다. 기본 수치는 500부터 시작
extern const int spawn_timer_r_max;	// 스폰 주기 최초 수치

extern int mdx_big;				  //대형 몬스터 수
extern int spawn_timer_big;
extern const int spawn_timer_big_max;

extern int mdx_air;				  //공중 몬스터 수
extern int spawn_timer_air;
extern const int spawn_timer_air_max;

//awp 관통의 조건: 최소 1마리 이상의 몬스터가 총알에 맞아야하고, 이후 is_hit이 TRUE가 되어 관통 효과 발동
extern BOOL is_hit;

int calc_spawn_timer(int spawn_timer_max);

void spawn_monster_regular(int mdx, int BG_scanner, RECT rt);
void spawn_monster_big(int mdx_big, int BG_scanner, RECT rt);
void spawn_monster_air(int mdx_air, int BG_scanner, RECT rt);

void update_monster_dir_r(double monster_x, double CM_x, int mdx);
void update_monster_dir_big(double monster_x, double CM_x, int mdx_big);
void update_monster_dir_air(double monster_x, double CM_x, int mdx_air);

void monster_hp_ind(HDC mdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

void monster_array_push_r(int hit, int idx);
void monster_array_push_big(int hit, int idx);
void monster_array_push_air(int i, int mdx_air);
void push_dead(int idx);