int space_pressed;       //점프 키 중복을 방지하기 위한 변수
int CM_move_dir = -1; // 0: 좌측이동, 1: 우측 이동 -1: 정지
int CM_jump_acc = 28; //점프 시 가속도
int CM_jump;          //0: 점프 안 함, 1: 점프 함 2: 착지 중

double CM_x, CM_y;    //플레이어 캐릭터의 위치

bool is_zoom = false;
bool avail_awp = false;