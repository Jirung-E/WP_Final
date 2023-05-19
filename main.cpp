#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "Vector2D.h"
#include "player_move.h" //�÷��̾� ���� ������ �����ϴ� ���
#include "images.h" //�̹��� ���� ������ �����ϴ� ���. ���, �Ϻ� ����, �Ϻ� ���⸦ ������ ������ �̹����� ũ��� 
                    //��� 100 x 100 px�� �ϵ��� ��. �̴� �۾� �� �� �� ���������� �۾��ϱ� ���� ����.

#include "Map.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

HDC hdc, mdc; PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt; //��� �޽������� �������� ����ϱ� ���� �ܿ� �������� ����

static int mx, my; //���콺 ��ǥ
static BOOL is_click = FALSE; //���콺 Ŭ�� ����

Map map { 16, 9 };		// ��ǥ��.  ��üȭ������� 16:9�� ����ϴ�.

enum Timer {
	KEYDOWN, UPDATE
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_CREATE: //�̹��� �ε� �� �ʱ� ������ ����
	{
		commando_right.Load(L".\\res\\commando_right.png");
		commando_left.Load(L".\\res\\commando_left.png");
		m1_right.Load(L".\\res\\m1_right.png");
		m1_left.Load(L".\\res\\m1_left.png");
		BackGround.Load(L".\\res\\BackGround.png");
		//�̹��� ���õ� �͵��� ��� images.h ���Ͽ� ����Ǿ�����
		//���ҽ� �̹����� ���Ե� 'res' ������ ���־� ��Ʃ��� �ַ�� ����(.sln)�� ���� ��ġ�� �־����.

		//SetTimer(hWnd, 0, 0, NULL); //KEYDOWN ���� Ÿ�̸�, �� Ÿ�̸ӿ� Ű���� �Է��� ������ ��� �ٸ��͵� �ۼ����� �� ��!

		SetTimer(hWnd, 1, 5, NULL); //���� ��ü Ÿ�̸�, ���� �ִϸ��̼� ���� Ÿ�̾ �߰� ����

		CM_x = 700, CM_y = 600; //�ʱ� �÷��̾� ��ġ
		CM_jump_acc = 28; //���� �� �������� ���ӵ�, �ٰų� �þ (WM_TIMER case 1 ����)
	}
		break;

	case WM_KEYDOWN:
		switch(wParam) {
		case VK_ESCAPE: //����
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_KEYUP:
		CM_move_dir = -1; //�������·� ����
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
		if (mx < CM_x + 50) //���콺 ��ǥ�� �÷��̾�� ���ʿ� ������ ������ �ٶ󺸰�, �����ʿ� ������ �������� �ٶ�
			CM_img_dir = 0;
		if (mx > CM_x + 50)
			CM_img_dir = 1;

		break;
	}

	case WM_TIMER:
		switch (wParam) {
		case KEYDOWN: //Ű���� �Է� ���� Ÿ�̸�. �̵��� ������ ���ÿ� �� �� ����.
		{
			if (GetAsyncKeyState('A') & 0x8000)  //���� �̵�
				CM_move_dir = 0;
			

			if (GetAsyncKeyState('D') & 0x8000)  //���� �̵�
				CM_move_dir = 1;
			

			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && space_down == 0) {
				if (space_down == 0) { //VK_SPACE�� ��� ������ ���� ��� �߻��� �� �ִ� �ߺ� �Է� ������ �����ϱ� ����
					CM_jump = 1;
					space_down = 1; //1�̸� VK_SPACE�� �Էµ��� ����
				}
			}
		}
		break;

		case UPDATE: //���� ��ü Ÿ�̸�
		//����
		{
			if (CM_jump == 1) { //���� �ö󰡴� ��
				CM_y -= CM_jump_acc; CM_jump_acc--; //���� �ö󰥼��� ������ �پ��
				if (CM_jump_acc == -1) { //������ ������ �پ���
					CM_jump_acc = 0; CM_jump = 2; //�������� ���� 
				}
			}
			else if (CM_jump == 2) { //�������� ��
				CM_y += CM_jump_acc; CM_jump_acc++; //�������鼭 ������ ������
				if (CM_jump_acc == 29) { //���� ������
					CM_jump_acc = 28; CM_jump = 0; space_down = 0; //�÷��̾�� ���� �����ϰ� VK_SPACE �Է��� ���� �غ� ��
				}
			}
		} 

		//�¿� �̵�
		{
			if (CM_move_dir == 0) { //���� �̵�
				CM_x -= 10;
				if (CM_x <= rt.left) //���� ������
					CM_x += 10; //�̵� ����
			}
			else if (CM_move_dir == 1) { //���� �̵�
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

			//////////////////////// ����
			BG_w = BackGround.GetWidth();
			BG_h = BackGround.GetHeight(); //��� �̹��� ������ �˾Ƴ� �� ����
			BackGround.Draw(mdc, 0, 0, rt.right, rt.bottom, 0, 0, BG_w, BG_h); //��� ��� �� ���� ���۸�

			map.draw(mdc, rt);			// ��� ���� ǥ�ÿ�
			

			//�̹��� ���
			{
				if (CM_img_dir == 0) { //�÷��̾�, ���� ���� �̹��� ���
					CM_w = commando_left.GetWidth();
					CM_h = commando_left.GetHeight();
					commando_left.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //�÷��̾� �̹��� ���

					if (GUN_number == 1) { // m1 garand_left
						GUN_w = m1_left.GetWidth();
						GUN_h = m1_left.GetWidth();
						m1_left.Draw(mdc, CM_x - 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h); //�ݵ�� �ѱ� ��ġ�� �÷��̾� '+-20'�� �������� ��
					}
				}
				else if (CM_img_dir == 1) {  //�÷��̾�, ���� ���� �̹��� ���
					CM_w = commando_right.GetWidth();
					CM_h = commando_right.GetHeight();
					commando_right.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //�÷��̾� �̹��� ���

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