#pragma once

#include <Windows.h>
#include <random>
#include <string>

using tstring = std::basic_string<TCHAR>;
static std::random_device rd;


double percentOf(double num, double per);
RECT percentOf(const RECT& rect, double per);


enum Color {
	White = 0xffffff, LightGray = 0xcccccc, Gray = 0x888888, DarkGray = 0x444444, Black = 0x000000,
	Red = 0x0000ff, Green = 0x00ff00, Blue = 0xff0000,
	Yellow = 0x00ffff, Cyan = 0xffff00, Magenta = 0xff00ff,
};

COLORREF mixColor(const COLORREF& color1, const COLORREF& color2);
COLORREF mixLight(const COLORREF& color1, const COLORREF& color2);
COLORREF brighter(const COLORREF& color, short int n = 10);


class Range {
public:
	double min;
	double max;

public:
	Range(double min, double max);
	Range(int min, int max);

public:
	bool isContain(double num) const;
};

double getRandomNumberOf(const Range& range, double interval = 1);
COLORREF getRandomColor();
COLORREF getRandomColor(const Range& r, const Range& g, const Range& b, const Range& base = { 0, 0 });
COLORREF getRandomColor(int r, int g, int b, const Range& base = { 0, 0xff });

enum Direction {
	Right, Down, Left, Up, None
};

POINT getCenterOf(const RECT& rect);
RECT rectToSquare(const RECT& rect, const Direction& bias = None);
RECT convertRatio(const RECT& rect, double width, double height, const Direction& bias = None);
RECT expandRatio(const RECT& rect, double width, double height, const Direction& bias = None);
RECT expand(const RECT& rect, int percentage);