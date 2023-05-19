#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "Vector2D.h"
#include "player_move.h" //플레이어 관련 변수를 저장하는 헤더
#include "images.h" //이미지 관련 변수를 저장하는 헤더. 배경, 일부 몬스터, 일부 무기를 제외한 나머지 이미지의 크기는 
                    //모두 100 x 100 px로 하도록 함. 이는 작업 시 좀 더 직관적으로 작업하기 위한 것임.

#include "Map.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

HDC hdc, mdc; PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt; //모든 메시지에서 공용으로 사용하기 위해 잔역 선언으로 변경

static int mx, my; //마우스 좌표
static BOOL is_click = FALSE; //마우스 클릭 여부

Map map { 16, 9 };		// 좌표계.  전체화면비율인 16:9에 맞춥니다.

enum Timer {
	KEYDOWN, UPDATE
};

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

		//SetTimer(hWnd, 0, 0, NULL); //KEYDOWN 전용 타이머, 이 타이머에 키보드 입력을 제외한 어떠한 다른것도 작성하지 말 것!

		SetTimer(hWnd, 1, 5, NULL); //게임 전체 타이머, 추후 애니메이션 전용 타이어도 추가 예정

		CM_x = 700, CM_y = 600; //초기 플레이어 위치
		CM_jump_acc = 28; //점프 시 가해지는 가속도, 줄거나 늘어남 (WM_TIMER case 1 참고)
	}
		break;

	case WM_KEYDOWN:
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
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
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
			

			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && space_down == 0) {
				if (space_down == 0) { //VK_SPACE를 계속 누르고 있을 경우 발생할 수 있는 중복 입력 오류를 방지하기 위함
					CM_jump = 1;
					space_down = 1; //1이면 VK_SPACE는 입력되지 않음
				}
			}
		}
		break;

		case UPDATE: //게임 전체 타이머
		//점프
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
		} 

		//좌우 이동
		{
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

		break;
		
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

			map.draw(mdc, rt);			// 배경 격자 표시용
			

			//이미지 출력
			{
				if (CM_img_dir == 0) { //플레이어, 무기 좌측 이미지 출력
					CM_w = commando_left.GetWidth();
					CM_h = commando_left.GetHeight();
					commando_left.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력

					if (GUN_number == 1) { // m1 garand_left
						GUN_w = m1_left.GetWidth();
						GUN_h = m1_left.GetWidth();
						m1_left.Draw(mdc, CM_x - 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h); //반드시 총기 위치는 플레이어 '+-20'을 기준으로 함
					}
				}
				else if (CM_img_dir == 1) {  //플레이어, 무기 우측 이미지 출력
					CM_w = commando_right.GetWidth();
					CM_h = commando_right.GetHeight();
					commando_right.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //플레이어 이미지 출력

					if (GUN_number == 1) { // m1 garand_right
						GUN_w = m1_right.GetWidth();
						GUN_h = m1_right.GetWidth();
						m1_right.Draw(mdc, CM_x + 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h);
					}
				}
			}

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