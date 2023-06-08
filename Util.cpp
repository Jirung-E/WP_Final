#include "Util.h"


double percentOf(double num, double per) {
	return num*per/100;
}

RECT percentOf(const RECT& rect, double per) {
	RECT r = rect;
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	r.left += w * (100 - per)/2 / 100;
	r.right -= w * (100 - per)/2 / 100;
	r.top += h * (100 - per)/2 / 100;
	r.bottom -= h * (100 - per)/2 / 100;

	return r;
}


COLORREF mixColor(const COLORREF& color1, const COLORREF& color2) {
	int r1 = color1 & Red;
	int g1 = (color1 & Green) / 0x000100;
	int b1 = (color1 & Blue) / 0x010000;
	int r2 = color2 & Red;
	int g2 = (color2 & Green) / 0x000100;
	int b2 = (color2 & Blue) / 0x010000;

	int max = 0;
	int red = r1 + r2;
	if(red > max) {
		max = red;
	}
	int green = g1 + g2;
	if(green > max) {
		max = green;
	}
	int blue = b1 + b2;
	if(blue > max) {
		max = blue;
	}

	int sub = 0;
	if(max > 255) {
		sub = max - 255;
	}

	red = red - sub;
	if(red < 0) {
		red = 0;
	}
	green = green - sub;
	if(green < 0) {
		green = 0;
	}
	blue = blue - sub;
	if(blue < 0) {
		blue = 0;
	}

	return RGB(red, green, blue);
}

COLORREF mixLight(const COLORREF& color1, const COLORREF& color2) {
	return color1 | color2;
}

COLORREF brighter(const COLORREF& color, short int n) {
	int red = color & Red;
	int green = (color & Green) / 0x000100;
	int blue = (color & Blue) / 0x010000;

	red += n;
	if(red > 255) {
		red = 255;
	}
	else if(red < 0) {
		red = 0;
	}
	green += n;
	if(green > 255) {
		green = 255;
	}
	else if(green < 0) {
		green = 0;
	}
	blue += n;
	if(blue > 255) {
		blue = 255;
	}
	else if(blue < 0) {
		blue = 0;
	}

	return RGB(red, green, blue);
}


COLORREF getRandomColor() {
	std::uniform_int_distribution<> random_color { 0 + 0x11, 0xFF - 0x11 };
	return RGB(random_color(rd), random_color(rd), random_color(rd));
}


COLORREF getRandomColor(const Range& r, const Range& g, const Range& b, const Range& base) {
	std::uniform_int_distribution<> r_base { int(base.min), int(base.max) };
	std::uniform_int_distribution<> r_r { int(r.min), int(r.max) };
	std::uniform_int_distribution<> r_g { int(g.min), int(g.max) };
	std::uniform_int_distribution<> r_b { int(b.min), int(b.max) };
	int bs = r_base(rd);
	return RGB(bs + r_r(rd), bs + r_g(rd), bs + r_b(rd));
}

COLORREF getRandomColor(int r, int g, int b, const Range& base) {
	std::uniform_int_distribution<> r_base { int(base.min), int(base.max) };
	int bs = r_base(rd);
	return RGB(r + bs, g + bs, b + bs);
}


POINT getCenterOf(const RECT& rect) {
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	return { rect.left + width/2, rect.top + height/2 };
}

RECT rectToSquare(const RECT& rect, const Direction& bias) {
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int n = width < height ? width/2 : height/2;
	POINT center = getCenterOf(rect);

	RECT r = { center.x-n, center.y-n, center.x+n, center.y+n };

	switch(bias) {
	case Left:
		r.left = rect.left;
		r.right = rect.left + 2*n;
		break;
	case Right:
		r.left = rect.right - 2*n;
		r.right = rect.right;
		break;
	case Up:
		r.top = rect.top;
		r.bottom = rect.top + 2*n;
		break;
	case Down:
		r.top = rect.bottom - 2*n;
		r.bottom = rect.bottom;
		break;
	}

	return r;
}

RECT convertRatio(const RECT& rect, double width, double height, const Direction& bias) {
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	RECT r = rect;
	if(width*h > height*w) {
		int n = h - height * w / width;
		switch(bias) {
		case Up:
			r.bottom -= n;
			break;
		case Down:
			r.top += n;
			break;
		case None:
			r.top += n/2;
			r.bottom -= n/2;
			break;
		}
	}
	else {
		int n = w - width * h / height;
		switch(bias) {
		case Left:
			r.right -= n;
			break;
		case Right:
			r.left += n;
			break;
		case None:
			r.left += n/2;
			r.right -= n/2;
			break;
		}
	}

	return r;
}

RECT expandRatio(const RECT& rect, double width, double height, const Direction& bias) {
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	RECT r = rect;
	if(width*h > height*w) {
		int n = width * h / height - w;
		switch(bias) {
		case Left:
			r.right += n;
			break;
		case Right:
			r.left -= n;
			break;
		case None:
			r.left -= n/2;
			r.right += n/2;
			break;
		}
	}
	else {
		int n = height * w / width - h;
		switch(bias) {
		case Up:
			r.bottom += n;
			break;
		case Down:
			r.top -= n;
			break;
		case None:
			r.top -= n/2;
			r.bottom += n/2;
			break;
		}
	}

	return r;
}

RECT expand(const RECT& rect, int percentage) {
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	RECT r;
	r.left = rect.left - w*percentage/100;
	r.right = rect.right + w*percentage/100;
	r.top = rect.top - h*percentage/100;
	r.bottom = rect.bottom + h*percentage/100;
	return r;
}


Range::Range(double min, double max) : min { min }, max { max } {

}

Range::Range(int min, int max) : Range { double(min), double(max) } {

}

bool Range::isContain(double num) const {
	if(min <= num && num <= max) {
		return true;
	}
	return false;
}


double getRandomNumberOf(const Range& range, double interval) {
	int count = (range.max - range.min) / interval;
	std::uniform_int_distribution<> uid { 0, count };
	return range.min + interval * uid(rd);
}