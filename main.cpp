#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "GameManager.h"
#include "monster_info.h"//몬스터 정보 헤더
#include "ammo.h"        //총알 정보 헤더
#include "player_info.h" //플레이어 정보 헤더
#include "images.h"      //이미지 정보 헤더. 배경, 일부 몬스터, 일부 무기를 제외한 나머지 이미지의 크기는 
                         //모두 100 x 100 px로 하도록 함. 이는 작업 시 좀 더 직관적으로 작업하기 위한 것임.
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

GameManager manager;
extern ammo_pos ap[500];      //총알의 위치 및 움직이는 여부를 저장하는 구조체, ammo.h에 선언
extern monster_info mst[100]; //몬스터 정보를 저정하는 구조체, monster_info.h에 선언

enum Timer {
	KEYDOWN, UPDATE
};

void IMG_FILE_LOAD() {
	BackGround.Load(L".\\res\\BackGround_wide.png");
	commando_right.Load(L".\\res\\commando_right.png");
	commando_left.Load(L".\\res\\commando_left.png");
	commando_jump_right.Load(L".\\res\\commando_jump_right.png");
	commando_jump_left.Load(L".\\res\\commando_jump_left.png");
	monster_right.Load(L".\\res\\monster_right.png");
	monster_left.Load(L".\\res\\monster_left.png");
	SCAR_H_right.Load(L".\\res\\SCAR_H_right.png");
	SCAR_H_left.Load(L".\\res\\SCAR_H_left.png");
	indicator_back.Load(L".\\res\\indicator_back.png");
	ammo_icon.Load(L".\\res\\ammo_icon.png");
} 

static double var; //총을 오래 사격할 수록 반동으로 인해 정확도가 떨어짐, 수치가 증가할 수록 분산도가 커짐
void show_target(HDC mdc, int mouse_x, int mouse_y, double var) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(mdc, hpen);

	MoveToEx(mdc, mouse_x + 10 + var, mouse_y, NULL);
	LineTo(mdc, mouse_x + 30 + var, mouse_y);
	MoveToEx(mdc, mouse_x - 10 - var, mouse_y, NULL);
	LineTo(mdc, mouse_x - 30 - var, mouse_y);
	MoveToEx(mdc, mouse_x, mouse_y - 10 - var, NULL);
	LineTo(mdc, mouse_x, mouse_y - 30 - var);
	MoveToEx(mdc, mouse_x, mouse_y + 10 + var, NULL);
	LineTo(mdc, mouse_x, mouse_y + 30 + var);

	SelectObject(mdc, oldpen);
	DeleteObject(hpen);
}

void show_ammo(HDC mdc, int mouse_x, int mouse_y) {
	for (int i = 0; i < apx; i++) //총알 출력
		draw_ammo(mdc, ap[i].x, ap[i].y, ap[i].x2, ap[i].y2); //ammo.h에 선언
}

void show_player(HDC mdc) {
	switch (CM_img_dir) { //플레이어, 총 이미지 출력
	case 0:
		if (CM_jump == 0) {
			CM_w = commando_left.GetWidth();
			CM_h = commando_left.GetHeight();
			commando_left.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			CM_w = commando_jump_left.GetWidth();
			CM_h = commando_jump_left.GetHeight();
			commando_jump_left.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
		}

		switch (GUN_number) {
		case scar_h:
			GUN_w = SCAR_H_left.GetWidth();
			GUN_h = SCAR_H_left.GetWidth();
			SCAR_H_left.Draw(mdc, CM_x - 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
			break;
		}

		break;
		//////////////////////
	case 1:
		if (CM_jump == 0) {
			CM_w = commando_right.GetWidth();
			CM_h = commando_right.GetHeight();
			commando_right.Draw(mdc, CM_x + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력
		}
		else if (CM_jump == 1 || CM_jump == 2) {
			CM_w = commando_jump_right.GetWidth();
			CM_h = commando_jump_right.GetHeight();
			commando_jump_right.Draw(mdc, CM_x + ss_x, CM_y - 10 + landing_shake + ss_y, 100, 120, 0, 0, CM_w, CM_h); //플레이어 점프 이미지 출력
		}

		switch (GUN_number) {
		case scar_h:
			GUN_w = SCAR_H_right.GetWidth();
			GUN_h = SCAR_H_right.GetWidth();
			SCAR_H_right.Draw(mdc, CM_x + 40 + ss_x, CM_y + landing_shake + ss_y, 100, 100, 0, 0, GUN_w, GUN_h);
			break;
		}

		break;
	}
}

void show_interface(HDC mdc, RECT rt) {
	//장탄수 표시기 배경
	IND_w = indicator_back.GetWidth();
	IND_h = indicator_back.GetHeight();
	indicator_back.Draw(mdc, rt.right - 600 + ss_x, rt.bottom - 110 + landing_shake + ss_y, 600, 110, 0, 0, IND_w, IND_h);

	//총 아이콘
	switch (GUN_number) {
	case scar_h:
		GUN_w = SCAR_H_right.GetWidth();
		GUN_h = SCAR_H_right.GetWidth();
		SCAR_H_right.Draw(mdc, rt.right - 430 + ss_x, rt.bottom - 150 + landing_shake + ss_y, 150, 150, 0, 0, GUN_w, GUN_h);
		break;
	}

	//총알 아이콘
	AMO_w = ammo_icon.GetWidth();
	AMO_h = ammo_icon.GetHeight();
	ammo_icon.Draw(mdc, rt.right - 260 + ss_x, rt.bottom - 108 + landing_shake + ss_y, 100, 100, 0, 0, AMO_w, AMO_h);

	//장탄 수 출력
	switch (GUN_number) { //ammo.h에 선언
	case scar_h:
		ammo_indicator(mdc, apx, scar_h, ind_size, ind_x + ss_x, ind_y + landing_shake + ss_y); //ammo.h에 선언
		break;
	}

	//재장전 게이지 출력
	if (reload == 1)
		reload_indicator(mdc, CM_x, CM_y - 30 + landing_shake, CM_x + reload_x, CM_y - 10 + landing_shake, CM_x, CM_y - 30 + landing_shake, CM_x + 100, CM_y - 10 + landing_shake);

}

void update_player_direction(int mouse_x) {
	if(mouse_x < CM_x + 50) //마우스 좌표가 플레이어보다 왼쪽에 있으면 왼쪽을 바라보고, 오른쪽에 있으면 오른쪽을 바라봄
		CM_img_dir = 0;
	if(mouse_x > CM_x + 50)
		CM_img_dir = 1;
}

void update_player_position(RECT rt) {
//점프
	if (CM_jump == 1) { //위로 올라가는 중
		CM_y -= CM_jump_acc; CM_jump_acc--; //위로 올라갈수록 가속이 줄어듬
		if (CM_jump_acc == -1) {            //가속이 완전히 줄어들면
			CM_jump_acc = 0; CM_jump = 2;   //떨어지기 시작 
		}
	}
	else if (CM_jump == 2) { //떨어지는 중
		CM_y += CM_jump_acc; CM_jump_acc++;                //떨어지면서 가속이 증가함
		if (CM_jump_acc == 29) { //땅에 닿으면
			CM_jump_acc = 28; CM_jump = 0; space_down = 0; //플레이어는 땅에 착지하고 VK_SPACE 입력을 받을 준비를 함
			is_land = 1;
		}
	}

	if (is_land == 1) { //점프 후 착지 시 화면에 흔들림 발생
		if (shake_reduce == 0) {
			landing_shake = 30; shake_reduce = 1; //화면 전체가 30씩 내려간다.
		}
		if (shake_reduce == 1) {
			landing_shake -= 2;                   //시간이 지나면서 점차 원래대로 돌아온다
			if (landing_shake == 0) {
				is_land = 0;
				shake_reduce = 0;
			}
		}
	}
	
//좌우 이동
	if (CM_move_dir == 0) { //좌측 이동
		if (BG_scanner >= 10 && CM_x == 700)  //배경 인식 좌표가 10보다 크고 플레이어 x 좌표가 700이면
			BG_scanner -= 15;                 //배경이 움직인다

		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2900 && CM_x >= 700)) //배경 인식 좌표가 10이되고 플레이어가 다시 가운데로 이동할 때까지
			CM_x -= 15;                       //플레이어만 움직인다

		if (CM_x <= rt.left)  //벽에 닿으면
			CM_x += 15;                 //이동 중지
	}

	else if (CM_move_dir == 1) { //우측 이동
		if (BG_scanner <= 2990 && CM_x == 700)
			BG_scanner += 15;

		if ((BG_scanner <= 10 && CM_x <= 700) || (BG_scanner >= 2900 && CM_x >= 700))
			CM_x += 15;

		if (CM_x + 100 >= rt.right)
			CM_x -= 15;
	}
	
}

void update_shoot_animation(RECT rt, int mouse_x, int mouse_y, BOOL is_click) {
	//사격
	if (is_click == TRUE && reload == 0) {
		switch (GUN_number) {
		case scar_h: //케이스 넘버에 define한 총 이름을 넣으면 됨
			if (apx < 25) {
				shoot_delay++;                                         //LBUTTON이 눌려있는 동안 딜레이 값이 계속 증가함
				if (shoot_delay == 6) {                                //딜레이 값이 정해진 값에 도달하면
					make_ammo(apx, CM_x, CM_y, mouse_x, mouse_y, var); //플레이어의 위치에 총알 객체를 만든다. 이때 반동으로 인해 분산도가 발생한다. ammo.h에 선언
					apx++; var += 4; ind_effect = 1; shake_effect = 1; //각각 인터페이스 이펙트, 흔들림 이펙트
					shoot_delay = 0;								   //딜레이는 0이되어 다시 딜레이가 증가하기 시작
					
					if (apx == 25) empty = 1;
				}
			}
			break;

		}
	}

	//사격 중지 시 조준점이 다시 회복됨
	if (is_click == FALSE || reload == 1 || empty == 1) {
		if (var >= 0) var -= 2;
	}

	for (int i = 0; i < apx; i++) //총알이 날아가는것을 그림
		ammo_move(i, ap[i].is_shoot, rt, BG_scanner); //ammo.h에 선언

	 //애니매이션
	{
		if (ind_effect == 1) { //인터페이스 애니메이션 활성화
			ind_ani = 1; ind_size = 150; ind_x = 1335; ind_y = 625;
			ind_effect = 0;
		}

		//장탄수, 재장전 인디케이터
		if (ind_ani == 1) { //장탄수 인디케이터 애니메이션
			ind_size -= 10; ind_x += 5; ind_y += 5;

			if (ind_size == 100) ind_ani = 0;
		}


		if (shake_effect == 1) { //사격 시 화면 흔들림
			std::random_device rd; std::mt19937 gen(rd());

			switch (GUN_number) { //총기마다 반동 수치가 달라 값을 달리해야 함
			case scar_h:
				shake_acc = 10; //SCAR_H의 경우 흔들림 가속 수치를 10을 줌
				std::uniform_int_distribution<int> shake_x(-shake_acc, shake_acc); //숫자가 클 수록 반동이 커 화면 흔들림이 격해짐
				std::uniform_int_distribution<int> shake_y(-shake_acc, shake_acc); //짧은 순간에 ss_x, ss_y에 랜덤한 값을 넣어 흔들림 표현
				ss_x = shake_x(gen); ss_y = shake_y(gen);						  
				shake_count++;          //몇 번 카운트를 세냐에 따라 화면 흔들리는 시간이 달라진다. 많이 세면 샐 수록 흔들리는 시간이 길어진다.
				shake_acc -=2;          //변경되는 좌표의 크기가 점차 줄면서 좀 더 자연스러운 흔들림을 만듬
				if (shake_count == 5) { //특정 횟수가 되면
					shake_count = 0;    //카운트 초기화
					ss_x = 0; ss_y = 0; //화면 좌표는 원래대로 돌아온다
					shake_effect = 0;   //더 이상 흔들리지 않는다.
					shake_acc = 10;     //흔들림 가속값 다시 초기화
				}
				break;
			}
		}

		if (reload == 1) { //재장전 인디케이터
			reload_x += 2; //재장전 인디케이터의 게이지가 채워진다
			if (reload_x + CM_x == CM_x + 100) { //모두 채워지면
				apx = 0;  reload = 0;  r_down = 0; reload_x = 0; empty = 0;
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, mdc;  PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt;
	static double mx, my;         //마우스 좌표
	static BOOL is_click = FALSE; //마우스 클릭 여부

	switch(uMsg) {
	case WM_CREATE: //이미지 로드 및 초기 변수값 세팅
		IMG_FILE_LOAD();        //이미지 로드 함수
		CM_x = 700, CM_y = 600; //초기 플레이어 위치
		ShowCursor(FALSE);      //커서 대신 조준점 보이기
		SetTimer(hWnd, KEYDOWN, 0, NULL); //KEYDOWN 전용 타이머, 이 타이머에 키보드 입력을 제외한 어떠한 다른것도 작성하지 말 것!
		SetTimer(hWnd, UPDATE, 5, NULL);  //게임 전체 타이머, 추후 애니메이션 전용 타이머도 추가 예정
		break;

	case WM_KEYDOWN:
		manager.keyboardInput(hWnd, wParam);
		break;

	case WM_KEYUP:
		CM_move_dir = -1; //정지상태로 변경
		break;

	case WM_LBUTTONDOWN:
		is_click = TRUE;
		if (empty == 1)
			reload = 1;
		//triggered = true;    //스크롤 배경 출력으로 인해 임시 비활성화

		switch (GUN_number) {  //총마다 연사속도가 다르므로 딜레이 수치를 다르게 줘야함. 연사력이 높을수록 딜레이 수치는 낮음
		case scar_h:
			shoot_delay = 5;   //타이머에서 검사하는 딜레이 수치보다 1 적게 초기화해야 발사됨
			break;
		}

		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Left);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
		//triggered = false; //스크롤 배경 출력으로 인해 임시 비활성화
		shoot_delay = 0;     //버튼을 떼면 딜레이 수치가 0으로 초기화 된다.
		break;

	case WM_RBUTTONDOWN:
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Right);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam); my = HIWORD(lParam);
		update_player_direction(mx);
		break;

	case WM_TIMER:
		switch(wParam) {
		case KEYDOWN: //키보드 입력 전용 타이머. 이동과 점프를 동시에 할 수 있음.
		{
			if(GetAsyncKeyState('A') & 0x8000)  //좌측 이동
				CM_move_dir = 0;

			if(GetAsyncKeyState('D') & 0x8000)  //우측 이동
				CM_move_dir = 1;

			if(GetAsyncKeyState('R') & 0x8000)  //재장전
				if(r_down == 0) {
					reload = 1;
					r_down = 1; //1이면 입력되지 않음, 중복 입력 방지
				}

			if(GetAsyncKeyState(VK_SPACE) & 0x8000 && space_down == 0) {
				if(space_down == 0) { //VK_SPACE를 계속 누르고 있을 경우 발생할 수 있는 중복 입력 오류를 방지하기 위함
					CM_jump = 1;
					space_down = 1;   //1이면 VK_SPACE는 입력되지 않음
				}
			}
		}
		break;

		case UPDATE: //게임 전체 타이머
			GetClientRect(hWnd, &rt);
			manager.update(hWnd);
			update_player_position(rt);
			update_shoot_animation(rt, mx, my, is_click);
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			GetClientRect(hWnd, &rt);
			mdc = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			(HBITMAP)SelectObject(mdc, hbitmap); 

			//////////////////////// 버퍼
			BG_w = 1500;//BackGround.GetWidth(); //윈도우 사이즈 만큼만 화면으로 출력함
			BG_h = BackGround.GetHeight(); //배경 이미지 사이즈 알아낸 후 저장
			BackGround.Draw(mdc, rt.left + ss_x, rt.top - 30 + landing_shake + ss_y, rt.right, rt.bottom + 30, BG_scanner, 0, BG_w, BG_h); //배경 출력 겸 더블 버퍼링 
			//BG_scanner가 클수록 배경은 오른쪽으로 이동하게 됨

			//manager.syncSize(hWnd); //스크롤 배경 출력을 위해 임시로 비활성화
			//manager.show(mdc);

			show_player(mdc);
			show_ammo(mdc, mx, my);
			show_interface(mdc, rt); //인터페이스 출력 함수
			show_target(mdc, mx + ss_x, my + ss_y + landing_shake, var);
			////////////////////////
			////////////////////////

			BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
		
			DeleteDC(mdc);
			DeleteObject(hbitmap);
			EndPaint(hWnd, &ps);
		}
		
		break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	WNDCLASSEX WndClass;
	MSG Message;

	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_EX_TOPMOST, 100, 50, 1500, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}