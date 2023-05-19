#pragma once

#include "Sprite.h"
#include "Point.h"
#include "Map.h"

#include <vector>


class Object {
private:
	std::vector<Sprite*> sprites;
	int current_anim_order;

public:
	std::vector<int> animation_order;
	Point position;
	Vector velocity;
	Vector accelerate;
	RectSize volume;

public:
	Object(const Point& position);

public:
	//void addSprite(const CImage& source);
	void addSprite(Sprite* source);
	void setSpriteSizePer(double size_per);

	void draw(const HDC& hdc, const RECT& valid_area, const Map& map) const;

	void setAnimationSpriteNext();

	POINT getAbsolutePosition(const RECT& valid_area, const Map& map) const;

	void move(const Vector& vector, const Map& map);
	void move(const Map& map);
};