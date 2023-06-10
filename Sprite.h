#pragma once

#include <Windows.h>
#include <atlImage.h>

#include "Util.h"
#include "Point.h"


class Sprite {
public:
	CImage source;
	Point position;
	double size_per;		// 0�̸� �Ⱥ���, 100�϶� �⺻������
	bool fix_ratio;

public:
	Sprite(const tstring& file_name);
	Sprite(const CImage& cimage);

public:
	void draw(const HDC& hdc, const RECT& draw_area) const;
};