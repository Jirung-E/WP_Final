#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "ammo.h" //총알 관련 기능을 하는 헤더
#include "target.h" //조준점 관련 기능을 하는 헤더
#include "player_info.h" //플레이어 관련 변수를 저장하는 헤더
#include "images.h" //이미지 관련 변수를 저장하는 헤더. 배경, 일부 몬스터, 일부 무기를 제외한 나머지 이미지의 크기는 
                    //모두 100 x 100 px로 하도록 함. 이는 작업 시 좀 더 직관적으로 작업하기 위한 것임.
#include "GameManager.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

GameManager manager;

extern ammo_pos ap[500]; //총알의 위치 및 움직이는 여부를 저장하는 구조체, ammo.h에 정의

enum Timer {
	KEYDOWN, UPDATE
};

#define scar_h 1  //총에 각각 번호 부여
#define m16 2
#define mp_44 3
#define mg_42 4
#define p90 5

HDC hdc, mdc; PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt; //모든 메시지에서 공용으로 사용하기 위해 잔역 선언으로 변경
static double mx, my; //마우스 좌표
static BOOL is_click = FALSE; //마우스 클릭 여부

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_CREATE: //이미지 로드 및 초기 변수값 세팅
	{
		commando_right.Load(L".\\res\\commando_right.png");
		commando_left.Load(L".\\res\\commando_left.png");
		m1_right.Load(L".\\res\\m1_right.png");
		m1_left.Load(L".\\res\\m1_left.png");
		BackGround.Load(L".\\res\\BackGround.png");
		//이미지 관련된 것들은 모두 images.h 파일에 선언되어있음
		//리소스 이미지가 포함된 'res' 폴더가 비주얼 스튜디오 솔루션 파일(.sln)과 같은 위치에 있어야함.

		SetTimer(hWnd, 0, 0, NULL); //KEYDOWN 전용 타이머, 이 타이머에 키보드 입력을 제외한 어떠한 다른것도 작성하지 말 것!
		//키보드 입력이 안되어 다시 활성화 시킴

		SetTimer(hWnd, 1, 5, NULL); //게임 전체 타이머, 추후 애니메이션 전용 타이머도 추가 예정

		CM_x = 700, CM_y = 600; //초기 플레이어 위치
		CM_jump_acc = 28; //점프 시 가해지는 가속도, 줄거나 늘어남 (WM_TIMER case UPDATE 참고)
		ShowCursor(FALSE); //커서 대신 조준점 보이기
	}
		break;

	case WM_KEYDOWN:
		manager.keyboardInput(hWnd, wParam);
		switch(wParam) {
		case VK_ESCAPE: //종료
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_KEYUP:
		CM_move_dir = -1; //정지상태로 변경
		break;

	case WM_LBUTTONDOWN:
		is_click = TRUE;

		switch (GUN_number) { //총마다 연사속도가 다르므로 딜레이 수치를 다르게 줘야함. 연사력이 높을수록 딜레이 수치는 낮음
		case scar_h:
			shoot_delay = 5; //타이머에서 검사하는 딜레이 수치보다 1 적게 초기화해야 발사됨
			break;
		}

		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Left);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
		shoot_delay = 0; //버튼을 떼면 딜레이 수치가 0으로 초기화 된다.
		break;

	case WM_RBUTTONDOWN:
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Right);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE:
	{
		mx = LOWORD(lParam); my = HIWORD(lParam);
		if (mx < CM_x + 50) //마우스 좌표가 플레이어보다 왼쪽에 있으면 왼쪽을 바라보고, 오른쪽에 있으면 오른쪽을 바라봄
			CM_img_dir = 0;
		if (mx > CM_x + 50)
			CM_img_dir = 1;

		break;
	}

	case WM_TIMER:
		switch (wParam) {
		case KEYDOWN: //키보드 입력 전용 타이머. 이동과 점프를 동시에 할 수 있음.
		{
			if (GetAsyncKeyState('A') & 0x8000)  //좌측 이동
				CM_move_dir = 0;
			
			if (GetAsyncKeyState('D') & 0x8000)  //우측 이동
				CM_move_dir = 1;

			if (GetAsyncKeyState('R') & 0x8000) //재장전
				if (r_down == 0) {
					reload = 1;
					r_down = 1; //1이면 입력되지 않음, 중복 입력 방지
				}
			
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && space_down == 0) {
				if (space_down == 0) { //VK_SPACE를 계속 누르고 있을 경우 발생할 수 있는 중복 입력 오류를 방지하기 위함
					CM_jump = 1;
					space_down = 1; //1이면 VK_SPACE는 입력되지 않음
				}
			}
		}
		break;

		case UPDATE: //게임 전체 타이머
			manager.update(hWnd);
        
		//점프, 좌우이동
		{
			if (CM_jump == 1) { //위로 올라가는 중
				CM_y -= CM_jump_acc; CM_jump_acc--; //위로 올라갈수록 가속이 줄어듬
				if (CM_jump_acc == -1) { //가속이 완전히 줄어들면
					CM_jump_acc = 0; CM_jump = 2; //떨어지기 시작 
				}
			}
			else if (CM_jump == 2) { //떨어지는 중
				CM_y += CM_jump_acc; CM_jump_acc++; //떨어지면서 가속이 증가함
				if (CM_jump_acc == 29) { //땅에 닿으면
					CM_jump_acc = 28; CM_jump = 0; space_down = 0; //플레이어는 땅에 착지하고 VK_SPACE 입력을 받을 준비를 함
				}
			}
	
			if (CM_move_dir == 0) { //좌측 이동
				CM_x -= 10;
				if (CM_x <= rt.left) //벽에 닿으면
					CM_x += 10; //이동 중지
			}
			else if (CM_move_dir == 1) { //우측 이동
				CM_x += 10;
				if (CM_x + 100 >= rt.right)
					CM_x -= 10;
			}
		}

		//사격
		switch (GUN_number) { 
		case scar_h: //케이스 넘버에 define한 총 이름을 넣으면 됨
			if (is_click == TRUE && reload == 0 && apx < 30) { //해당 총기의 장탄 수 만큼 인덱스 증가에 제한을 둠, 절대로 apx를 엉뚱한 곳에서 초기화 해서는 안됨.
				shoot_delay++; //LBUTTON이 눌려있는 동안 딜레이 값이 계속 증가함
				if (shoot_delay == 6) { //딜레이 값이 정해진 값에 도달하면
					make_ammo(apx, CM_x, CM_y, mx, my, var); //플레이어의 위치에 총알 객체를 만든다. 이때 반동으로 인해 분산도가 발생한다. ammo.h에 선언
					apx++; //총알의 위치, 각도, 움직임 여부를 저장하는 인덱스를 1 증가
					var += 3; //반동으로 인해 조준점이 넓어진다.
					shoot_delay = 0; //딜레이는 0이되어 다시 딜레이가 증가하기 시작

					ind_ani = 1; //장탄수 인디케이터의 애니메이션 활성화
					ind_size = 150;
					ind_x = 1305;
					ind_y = 615;
				}
			}
			break;
		}

		//사격 중지 시 조준점이 다시 회복됨
		if (is_click == FALSE || reload == 1) {
			if(var >= 0)
				var--;
		}
		
		for (int i = 0; i < apx; i++) //총알이 날아가는것을 그림
			ammo_move(i, ap[i].is_shoot, rt); //ammo.h에 선언

		//장탄수, 재장전 인디케이터
		{
			if (ind_ani == 1) { //장탄수 인디케이터 애니메이션
				ind_size -= 10;
				ind_x += 5;
				ind_y += 5;

				if (ind_size == 100)
					ind_ani = 0;
			}

			if (reload == 1) { //재장전 인디케이터
				reload_x += 2; //재장전 인디케이터의 게이지가 채워진다
				if (reload_x + CM_x == CM_x + 100) { //모두 채워지면
					apx = 0; //총알 인덱스는 0으로 초기화
					reload = 0; //재장전 완료
					r_down = 0;
					reload_x = 0;
				}
			}
		}

		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			GetClientRect(hWnd, &rt);
			HDC mdc = CreateCompatibleDC(hdc);
			HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			(HBITMAP)SelectObject(mdc, hbitmap); 

			//////////////////////// 버퍼
			BG_w = BackGround.GetWidth();
			BG_h = BackGround.GetHeight(); //배경 이미지 사이즈 알아낸 후 저장
			BackGround.Draw(mdc, 0, 0, rt.right, rt.bottom, 0, 0, BG_w, BG_h); //배경 출력 겸 더블 버퍼링

			manager.syncSize(hWnd);
			manager.show(mdc);
			

			//이미지 출력
			switch (CM_img_dir) {
			case 0:
				CM_w = commando_left.GetWidth();
				CM_h = commando_left.GetHeight();
				commando_left.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력

				switch (GUN_number) {
				case scar_h:
					GUN_w = m1_left.GetWidth();
					GUN_h = m1_left.GetWidth();
					m1_left.Draw(mdc, CM_x - 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
					break;
				}

				break;

			//////////////////////

			case 1:
				CM_w = commando_right.GetWidth();
				CM_h = commando_right.GetHeight();
				commando_right.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력

				switch (GUN_number) {
				case scar_h:
					GUN_w = m1_right.GetWidth();
					GUN_h = m1_right.GetWidth();
					m1_right.Draw(mdc, CM_x + 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h);
					break;
				}

				break;
			}
				
			//총알 출력
			for (int i = 0; i < apx; i++)
				draw_ammo(mdc, ap[i].x, ap[i].y, ap[i].x2, ap[i].y2); //ammo.h에 선언

			//타겟 출력
			draw_target(mdc, mx, my, var); //target.h에 선언
			
			//장탄 수 출력
			switch (GUN_number) { //ammo.h에 선언
			case scar_h: 
				ammo_indicator(mdc, apx, scar_h, ind_size, ind_x, ind_y); //ammo.h에 선언
				break;
			}

			//재장전 게이지 출력
			if(reload == 1)
				reload_indicator(mdc, CM_x, CM_y - 30, CM_x + reload_x, CM_y - 10, CM_x, CM_y - 30, CM_x + 100, CM_y - 10);

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