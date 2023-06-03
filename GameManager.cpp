#include "GameManager.h"

#include <fstream>

#include "exp.h"
#include "images.h"

using namespace std;


GameManager::GameManager() : main_scene { }, game_scene { }, armory_scene { },
current_scene { &main_scene }, mouse_position { 0, 0 } {
	ifstream ifs { "userdata.txt" };
	if(!ifs.fail()) {
		// 열렸으면 일단 예외 처리 없이 쭉 받기
		ifs >> experience;
		ifs >> GUN_number;
		armory_scene.unlocked[GUN_number-1] = true;
		while(!ifs.eof()) {
			int id;
			ifs >> id;
			armory_scene.unlocked[id-1] = true;
		}
	}

	// 파일 포맷:
	// 경험치
	// 현재 총(enum값 그대로 저장)
	// 보유한 총1
	// 보유한 총2
	// ...
}

GameManager::~GameManager() {
	ofstream ofs { "userdata.txt" };
	if(!ofs.fail()) {
		ofs << experience << endl;
		ofs << GUN_number << endl;
		int i=0;
		for(auto e : armory_scene.unlocked) {
			i++;
			if(e == true) {
				ofs << i << endl;
			}
		}
	}
}


void GameManager::show(const HDC& hdc) {
	current_scene->show(hdc);
}

void GameManager::syncSize(const HWND& hWnd) {
	current_scene->syncSize(hWnd);
	syncMousePosition(hWnd);
}


void GameManager::keyboardInput(const HWND& hWnd, int keycode) {
	switch(current_scene->getID()) {
	case Main:
		switch(keycode) {
		case VK_ESCAPE:
			quit(hWnd);
			break;
		}
		break;
	case Game:
		switch(keycode) {
		case VK_ESCAPE:
			game_scene.togglePauseState();
			releaseCursor();
			break;
		case L'a': case L'A':
			game_scene.moveLeft();
			break;
		case L'd': case L'D':
			game_scene.moveRight();
			break;
		}
		break;
	case Armory:
		switch(keycode) {
		case VK_ESCAPE:
			quit(hWnd);
			break;
		}
		break;
	}
}


void GameManager::clickScene(const HWND& hWnd, const POINT& point, const Direction& dir) {
	switch(dir) {
	case Left:
		switch(current_scene->getID()) {
		case Main:
			switch(buttonClicked(point)) {
			case MainScene::Start:
				gameStart(hWnd);
				break;
			case MainScene::Quit:
				PostQuitMessage(0);
				break;
			case MainScene::Armory:
				armory_scene.selected_weapon_button_index = -1;
				current_scene = &armory_scene;
				break;
			}
			break;
		case Game:
			switch(buttonClicked(point)) {
			case GameScene::Resume:
				game_scene.resume();
				lockCursor(hWnd);
				break;
			case GameScene::Quit:
				quit(hWnd);
				break;
			}
			break;
		case Armory:
			switch(buttonClicked(point)) {
			case ArmoryScene::Quit:
				current_scene = &main_scene;
				break;
			}
			break;
		}
		break;
	case Right:
		switch(current_scene->getID()) {
		case Game:
			game_scene.clickR(point);
			break;
		}
		break;
	}
}

void GameManager::update(const HWND& hWnd) {
	switch(current_scene->getID()) {
	case Game:
		game_scene.update(mouse_position);
		break;
	case Main:

		break;
	}
}

void GameManager::quit(const HWND& hWnd) {
	switch(current_scene->getID()) {
	case Game:
		releaseCursor();
	case Armory:
		current_scene = &main_scene;
		break;
	case Main:
		PostQuitMessage(0);
		break;
	}
}


void GameManager::setTimers(const HWND& hWnd) {
	SetTimer(hWnd, Display, fps(60), NULL);
	SetTimer(hWnd, UpdateGame, fps(60), NULL);
}

void GameManager::timer(const HWND& hWnd, int id) {
	switch(id) {
	case Display:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case UpdateGame:
		update(hWnd);
		break;
	}
}

void GameManager::interrupt() {
	releaseCursor();
	switch(current_scene->getID()) {
	case Game:
		game_scene.pause();
		break;
	}
}


SceneID GameManager::getCurrentSceneID() const {
	return current_scene->getID();
}

bool GameManager::isPaused() const {
	switch(current_scene->getID()) {
	case Game:
		return game_scene.isPaused();
	}
	return false;
}


void GameManager::gameStart(const HWND& hWnd) {
	fixCursor(hWnd);

	game_scene.setUp();
	current_scene = &game_scene;

	ShowCursor(FALSE);                //커서 대신 조준점 보이기
}

void GameManager::lockCursor(const HWND& hWnd) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	POINT lt = { rect.left, rect.top };
	ClientToScreen(hWnd, &lt);
	rect.left += lt.x;
	rect.top += lt.y;
	rect.right += lt.x;
	rect.bottom += lt.y;
	ClipCursor(&rect);
}

void GameManager::fixCursor(const HWND& hWnd) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	POINT lt = { rect.left, rect.top };
	ClientToScreen(hWnd, &lt);
	rect.left += lt.x;
	rect.top += lt.y;
	rect.right += lt.x;
	rect.bottom += lt.y;
	ClipCursor(&rect);
	SetCursorPos((rect.left + rect.right)/2, (rect.top + rect.bottom)/2);
}

void GameManager::releaseCursor() {
	ClipCursor(NULL);
}

int GameManager::buttonClicked(const POINT& point) {
	switch(current_scene->getID()) {
	case SceneID::Main:
		return main_scene.clickL(point);
	case SceneID::Game:
		return game_scene.clickL(point);
	case SceneID::Armory:
		return armory_scene.clickL(point);
	default:
		return 0;
	}
}

void GameManager::syncMousePosition(const HWND& hWnd) {
	GetCursorPos(&mouse_position);
	ScreenToClient(hWnd, &mouse_position);
}