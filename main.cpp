#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "ammo.h" //�Ѿ� ���� ����� �ϴ� ���
#include "target.h" //������ ���� ����� �ϴ� ���
#include "player.h" //�÷��̾� ���� ������ �����ϴ� ���
#include "images.h" //�̹��� ���� ������ �����ϴ� ���. ���, �Ϻ� ����, �Ϻ� ���⸦ ������ ������ �̹����� ũ��� 
                    //��� 100 x 100 px�� �ϵ��� ��. �̴� �۾� �� �� �� ���������� �۾��ϱ� ���� ����.
#include "GameManager.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"NON STOP";

GameManager manager;

extern ammo_pos ap[500]; //�Ѿ��� ��ġ �� �����̴� ���θ� �����ϴ� ����ü, ammo.h�� ����

enum Timer {
	KEYDOWN, UPDATE
};

#define scar_h 1  //�ѿ� ���� ��ȣ �ο�
#define m16 2
#define mp_44 3
#define mg_42 4
#define p90 5

HDC hdc, mdc; PAINTSTRUCT ps; HBITMAP hbitmap; RECT rt; //��� �޽������� �������� ����ϱ� ���� �ܿ� �������� ����
static double mx, my; //���콺 ��ǥ
static BOOL is_click = FALSE; //���콺 Ŭ�� ����

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

		SetTimer(hWnd, 0, 0, NULL); //KEYDOWN ���� Ÿ�̸�, �� Ÿ�̸ӿ� Ű���� �Է��� ������ ��� �ٸ��͵� �ۼ����� �� ��!
		//Ű���� �Է��� �ȵǾ� �ٽ� Ȱ��ȭ ��Ŵ

		SetTimer(hWnd, 1, 5, NULL); //���� ��ü Ÿ�̸�, ���� �ִϸ��̼� ���� Ÿ�̸ӵ� �߰� ����

		CM_x = 700, CM_y = 600; //�ʱ� �÷��̾� ��ġ
		CM_jump_acc = 28; //���� �� �������� ���ӵ�, �ٰų� �þ (WM_TIMER case UPDATE ����)
		ShowCursor(FALSE); //Ŀ�� ��� ������ ���̱�
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

		switch (GUN_number) { //�Ѹ��� ����ӵ��� �ٸ��Ƿ� ������ ��ġ�� �ٸ��� �����. ������� �������� ������ ��ġ�� ����
		case scar_h:
			shoot_delay = 5; //Ÿ�̸ӿ��� �˻��ϴ� ������ ��ġ���� 1 ���� �ʱ�ȭ�ؾ� �߻��
			break;
		}

		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Left);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		is_click = FALSE;
		shoot_delay = 0; //��ư�� ���� ������ ��ġ�� 0���� �ʱ�ȭ �ȴ�.
		break;

	case WM_RBUTTONDOWN:
		manager.clickScene(hWnd, { LOWORD(lParam), HIWORD(lParam) }, Right);
		InvalidateRect(hWnd, NULL, FALSE);
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

			if (GetAsyncKeyState('R') & 0x8000) //������
				if (r_down == 0) {
					reload = 1;
					r_down = 1; //1�̸� �Էµ��� ����, �ߺ� �Է� ����
				}
			
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && space_down == 0) {
				if (space_down == 0) { //VK_SPACE�� ��� ������ ���� ��� �߻��� �� �ִ� �ߺ� �Է� ������ �����ϱ� ����
					CM_jump = 1;
					space_down = 1; //1�̸� VK_SPACE�� �Էµ��� ����
				}
			}
		}
		break;

		case UPDATE: //���� ��ü Ÿ�̸�
		//����, �¿��̵�
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

		//���
		switch (GUN_number) { 
		case scar_h: //���̽� �ѹ��� define�� �� �̸��� ������ ��
			if (is_click == TRUE && reload == 0 && apx < 30) { //�ش� �ѱ��� ��ź �� ��ŭ �ε��� ������ ������ ��, ����� apx�� ������ ������ �ʱ�ȭ �ؼ��� �ȵ�.
				shoot_delay++; //LBUTTON�� �����ִ� ���� ������ ���� ��� ������
				if (shoot_delay == 6) { //������ ���� ������ ���� �����ϸ�
					make_ammo(apx, CM_x, CM_y, mx, my, var); //�÷��̾��� ��ġ�� �Ѿ� ��ü�� �����. �̶� �ݵ����� ���� �л굵�� �߻��Ѵ�. ammo.h�� ����
					apx++; //�Ѿ��� ��ġ, ����, ������ ���θ� �����ϴ� �ε����� 1 ����
					var += 3; //�ݵ����� ���� �������� �о�����.
					shoot_delay = 0; //�����̴� 0�̵Ǿ� �ٽ� �����̰� �����ϱ� ����

					ind_ani = 1; //��ź�� �ε��������� �ִϸ��̼� Ȱ��ȭ
					ind_size = 150;
					ind_x = 1305;
					ind_y = 615;
				}
			}
			break;
		}

		//��� ���� �� �������� �ٽ� ȸ����
		if (is_click == FALSE || reload == 1) {
			if(var >= 0)
				var--;
		}
		
		for (int i = 0; i < apx; i++) //�Ѿ��� ���ư��°��� �׸�
			ammo_move(i, ap[i].is_shoot, rt); //ammo.h�� ����

		//��ź��, ������ �ε�������
		{
			if (ind_ani == 1) { //��ź�� �ε������� �ִϸ��̼�
				ind_size -= 10;
				ind_x += 5;
				ind_y += 5;

				if (ind_size == 100)
					ind_ani = 0;
			}

			if (reload == 1) { //������ �ε�������
				reload_x += 2; //������ �ε��������� �������� ä������
				if (reload_x + CM_x == CM_x + 100) { //��� ä������
					apx = 0; //�Ѿ� �ε����� 0���� �ʱ�ȭ
					reload = 0; //������ �Ϸ�
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

			//////////////////////// ����
			BG_w = BackGround.GetWidth();
			BG_h = BackGround.GetHeight(); //��� �̹��� ������ �˾Ƴ� �� ����
			BackGround.Draw(mdc, 0, 0, rt.right, rt.bottom, 0, 0, BG_w, BG_h); //��� ��� �� ���� ���۸�

			manager.syncSize(hWnd);
			manager.show(mdc);			// ��� ���� ǥ�ÿ�
			

			//�̹��� ���
			switch (CM_img_dir) {
			case 0:
				CM_w = commando_left.GetWidth();
				CM_h = commando_left.GetHeight();
				commando_left.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //�÷��̾� �̹��� ���

				switch (GUN_number) {
				case scar_h:
					GUN_w = m1_left.GetWidth();
					GUN_h = m1_left.GetWidth();
					m1_left.Draw(mdc, CM_x - 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h); //�ݵ�� �ѱ� ��ġ�� �÷��̾� '+-20'�� �������� ��
					break;
				}

				break;

			//////////////////////

			case 1:
				CM_w = commando_right.GetWidth();
				CM_h = commando_right.GetHeight();
				commando_right.Draw(mdc, CM_x, CM_y, 100, 100, 0, 0, CM_w, CM_h); //�÷��̾� �̹��� ���

				switch (GUN_number) {
				case scar_h:
					GUN_w = m1_right.GetWidth();
					GUN_h = m1_right.GetWidth();
					m1_right.Draw(mdc, CM_x + 20, CM_y, 100, 100, 0, 0, GUN_w, GUN_h);
					break;
				}

				break;
			}
				
			//�Ѿ� ���
			for (int i = 0; i < apx; i++)
				draw_ammo(mdc, ap[i].x, ap[i].y, ap[i].x2, ap[i].y2); //ammo.h�� ����

			//Ÿ�� ���
			draw_target(mdc, mx, my, var); //target.h�� ����
			
			//��ź �� ���
			switch (GUN_number) { //ammo.h�� ����
			case scar_h: 
				ammo_indicator(mdc, apx, scar_h, ind_size, ind_x, ind_y); //ammo.h�� ����
				break;
			}

			//������ ������ ���
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