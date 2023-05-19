#include "Sprite.h"

//#pragma comment (lib, "msimg32.lib")
#pragma comment(lib, "gdiplus.lib")
#include <gdiplus.h>

using namespace Gdiplus;


//Sprite::Sprite(const CImage& source) : source { source },
//position { }, size_per { 100 } {
//
//}

Sprite::Sprite(const tstring& file_name) : position { }, size_per { 100 }, flip_x { true }, flip_y { false } {
	source.Load(file_name.c_str());
}


void Sprite::draw(const HDC& hdc, const RECT& draw_area) {
	//RECT area = draw_area;

	//https://poik.tistory.com/442

	//int w = source.GetWidth();
	//int h = source.GetHeight();

	//CImage* pImage = new CImage;
	//pImage->Create(w, h, 24);

	//for(int i=0; i<h; ++i) {
	//	int y0 = i;
	//	if(flip_y) {
	//		y0 = h-i-1;
	//	}

	//	for(int k=0; k<w; ++k) {
	//		int x0 = k;
	//		if(flip_x) {
	//			x0 = w-k-1;
	//		}

	//		int xs = x0;
	//		int ys = y0;
	//		//byte* ps = (byte*)source.GetPixelAddress(xs, ys);
	//		//byte* p = (byte*)pImage->GetPixelAddress(k, i);
	//		pImage->SetPixel(k, i, source.GetPixel(xs, ys));
	//		//*p = *ps;
	//		//*(p + 1) = *(ps + 1);
	//		//*(p + 2) = *(ps + 2);
	//	}
	//}

	//pImage->Draw(hdc, draw_area);

	source.Draw(hdc, draw_area);
}