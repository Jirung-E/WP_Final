#pragma once

#include <Windows.h>
#include <atlImage.h>

#include "Util.h"
#include "Point.h"


class Sprite {
public:
	CImage source;
	Point position;
	//double rotate;	// 회전
	//Point offset;		// 이미지 위치 조정
	bool flip_x;		// 뒤집기
	bool flip_y;
	double size_per;		// 0이면 안보임, 100일때 기본사이즈
	bool fix_ratio;

public:
	Sprite(const tstring& file_name);

public:
	void draw(const HDC& hdc, const RECT& draw_area) const;
};