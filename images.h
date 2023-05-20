#pragma once
static CImage commando_right, commando_left, SCAR_H_right, SCAR_H_left, BackGround, indicator_back, ammo_icon;
//아직은 개발 테스트용 이미지로, 완료 직전에 좀 더 다듬은 이미지로 교체 예정
//_right: 우측 이미지, _left: 좌측 이미지
//backGround: 버퍼용 이미지로, 배경의 역할을 겸함. 아직은 테스트용 흰 배경으로, 추후 다른 이미지로 교체 예정

//배경 이미지 관련 (background)
static int BG_w, BG_h; //배경이미지의 가로, 세로 사이즈

//플레이어 이미지 관련 (commando)
static int CM_img_dir; //(0: 좌측, 1: 우측), 초기값은 0(좌측), 무기 이미지 방향도 이 변수를 사용함
static int CM_w, CM_h; //플레이어 캐릭터 이미지의 가로, 세로 사이즈

//무기 이미지 관련 (gun)
static int GUN_number = 1; //무기 번호
static int GUN_w, GUN_h; //무기(총) 이미지의 가로, 세로 사이즈
//무기의 위치 및 방향은 플레이어를 기준으로 하기 때문에 별도의 좌표 변수는 없음

static int BG_scanner; //배경 읽기 시작하는 좌표, 수치가 클 수록 배경은 오른쪽으로 이동한다.
static int landing_shake; //플레이어 착지 시 흔드림 표현, 모든 오브젝트의 y좌표에 더한다.
static int shake_reduce; //1이면 착지 후 화면이 복구, 0이면 끝
static int is_land; //착지 여부 검사

static int IND_w, IND_h; //장탄수 표시기 배경 이미지 크기
static int AMO_w, AMO_h; //장탄수 표시기에 표시되는 아이콘 이미지 크기

static int ss_x; //사격 시 화면 흔들림, 모든 오브젝트의 x좌표에 더한다. //(shoot shake_x)
static int ss_y; //사격 시 화면 흔들림, 모든 오브젝트의 y좌표에 더한다. //(shoot_shake_y)
static int shake_count; //흔들리는 횟수를 센다
static int ind_effect;  //1이면 활성화, 0이면 비활성화
static int shake_effect;