#pragma once

class Vector2D {
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D operator+(Vector2D vector) const;
	Vector2D operator-(Vector2D vector) const;
	Vector2D operator*(float n) const;
	Vector2D operator/(float n) const;
	Vector2D operator+=(Vector2D vector);
	Vector2D operator-=(Vector2D vector);
	Vector2D operator*=(float n);
	Vector2D operator/=(float n);
};