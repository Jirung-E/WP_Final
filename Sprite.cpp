#include "Sprite.h"

#pragma comment(lib, "gdiplus.lib")
#include <gdiplus.h>

using namespace Gdiplus;


Sprite::Sprite(const tstring& file_name) : position { }, size_per { 100 }, flip_x { true }, flip_y { false } {
	source.Load(file_name.c_str());
}


void Sprite::draw(const HDC& hdc, const RECT& draw_area) {
	source.Draw(hdc, draw_area);
}