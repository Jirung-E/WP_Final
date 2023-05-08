#include "Vector2D.h"


// <Vector2D> -----------------------------------------------------------------------------------------------------------------
Vector2D::Vector2D() : x { 0.0f }, y { 0.0f } {

}

Vector2D::Vector2D(float x, float y) : x { x }, y { y } {

}


Vector2D Vector2D::operator+(Vector2D vector) const {
	return Vector2D { x + vector.x, y + vector.y };
}

Vector2D Vector2D::operator-(Vector2D vector) const {
	return Vector2D { x - vector.x, y - vector.y };
}

Vector2D Vector2D::operator*(float n) const {
	return Vector2D { x * n, y * n };
}

Vector2D Vector2D::operator/(float n) const {
	return Vector2D { x / n, y / n };
}

Vector2D Vector2D::operator+=(Vector2D vector) {
	*this = *this + vector;
	return *this;
}

Vector2D Vector2D::operator-=(Vector2D vector) {
	*this = *this - vector;
	return *this;
}

Vector2D Vector2D::operator*=(float n) {
	*this = *this * n;
	return *this;
}

Vector2D Vector2D::operator/=(float n) {
	*this = *this / n;
	return *this;
}
// </Vector2D> ----------------------------------------------------------------------------------------------------------------