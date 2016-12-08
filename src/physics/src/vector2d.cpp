#include "vector2d.h"
#include <cmath>

namespace physics {

Vector2D::Vector2D() : x(), y() {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

bool Vector2D::operator==(Vector2D rhs) {
	return(x == rhs.x && y == rhs.y);
}

Vector2D Vector2D::operator+(Vector2D rhs) {
	return Vector2D(x + rhs.x, y + rhs.y);
}

Vector2D Vector2D::operator-(Vector2D rhs) {
	return Vector2D(x - rhs.x, y - rhs.y);
}

Vector2D Vector2D::operator+(double scalar) {
	return Vector2D(x + scalar, y + scalar);
}

Vector2D Vector2D::operator-(double scalar) {
	return Vector2D(x - scalar, y - scalar);
}

Vector2D Vector2D::operator*(double scalar) {
	return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(double scalar) {
	return Vector2D(x / scalar, y / scalar);
}

double Vector2D::dot(Vector2D rhs) {
	return (x * rhs.x + y * rhs.y);
}

double Vector2D::magnitude() {
	return sqrt(x*x + y*y);
}

double Vector2D::distance(Vector2D other) {
	return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}

}
