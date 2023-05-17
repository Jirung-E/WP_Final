#pragma once
static CImage commando_right, commando_left, m1_right, m1_left, BackGround;
//아직은 개발 테스트용 이미지로, 완료 직전에 좀 더 다듬은 이미지로 교체 예정
//_right: 우측 이미지, _left: 좌측 이미지
//backGround: 버퍼용 이미지로, 배경의 역할을 겸함. 아직은 테스트용 흰 배경으로, 추후 다른 이미지로 교체 예정

//배경 이미지 관련 (background)
static int BG_w, BG_h; //배경이미지의 가로, 세로 사이즈

//플레이어 이미지 관련 (commando)
static int CM_img_dir; //(0: 좌측, 1: 우측), 초기값은 0(좌측), 무기 이미지 방향도 이 변수를 사용함
static int CM_w, CM_h; //플레이어 캐릭터 이미지의 가로, 세로 사이즈
static int CM_x, CM_y; //플레이어 캐릭터의 위치

//무기 이미지 관련 (gun)
static int GUN_number = 1; //무기 추가가 되면 번호가 더 생길 예정, 아니면 좀 더 직관적인 방법이 있으면 수정 바람.
static int GUN_w, GUN_h; //무기(총) 이미지의 가로, 세로 사이즈
//무기의 위치는 플레이어 좌표를 기준으로 하기 때문에 별도의 좌표 변수는 없음
