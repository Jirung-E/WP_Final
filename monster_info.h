#pragma once
struct monster_info { //몬스터의 보는 방향, 이동 방향, 위치를 저장하는 구조체
	double x, y;
	int move_dir;
	int img_dir;
};
static int MST_count; //몬스터 수
