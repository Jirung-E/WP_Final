#include "Sprite.h"


Sprite::Sprite(const tstring& file_name) : position { }, size_per { 100 }, fix_ratio { false } {
	source.Load(file_name.c_str());
}


void Sprite::draw(const HDC& hdc, const RECT& draw_area) const {
	RECT rect = percentOf(draw_area, size_per);
	if(fix_ratio) {
		rect = convertRatio(rect, source.GetWidth(), source.GetHeight());
	}
	rect.left += position.x;
	rect.right += position.x;
	rect.top += position.y;
	rect.bottom += position.y;
	source.Draw(hdc, rect);
}