#include "Sprite.h"


Sprite::Sprite(const tstring& file_name) : position { }, size_per { 100 }, fix_ratio { false } {
	source.Load(file_name.c_str());
}

Sprite::Sprite(const CImage& cimage) : source { cimage }, position { }, size_per { 100 }, fix_ratio { false } {

}


void Sprite::draw(const HDC& hdc, const RECT& draw_area) const {
	RECT rect = draw_area;
	if(fix_ratio) {
		rect = convertRatio(rect, source.GetWidth(), source.GetHeight());
	}
	rect.left += position.x;
	rect.right += position.x;
	rect.top += position.y;
	rect.bottom += position.y;
	source.Draw(hdc, rect);
}