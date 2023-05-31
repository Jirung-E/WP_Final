#include "Sprite.h"

#pragma comment(lib, "gdiplus.lib")
#include <gdiplus.h>

using namespace Gdiplus;


Sprite::Sprite(const tstring& file_name) : position { }, size_per { 100 }, 
flip_x { true }, flip_y { false }, fix_ratio { false } {
	source.Load(file_name.c_str());
}


void Sprite::draw(const HDC& hdc, const RECT& draw_area) const {
	RECT rect = draw_area;
	if(fix_ratio) {
		rect = convertRatio(rect, source.GetWidth(), source.GetHeight());
	}
	source.Draw(hdc, rect);
}