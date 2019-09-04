#include "geometry.h"

#include <algorithm>
#include <cmath>

#include <iostream>

namespace Geometry {

Vector::Vector() {
	x = 0;
	y = 0;
}

Vector::Vector(double _x, double _y) {
	x = _x;
	y = _y;
}


Vector operator+(const Vector& first, const Vector& second) {
	return Vector(first.x + second.x, first.y + second.y);
}

Vector operator-(const Vector& first, const Vector& second) {
	return Vector(first.x - second.x, first.y - second.y);
}

Vector operator*(const Vector& first, double k) {
	return Vector(first.x * k, first.y * k);
}

Vector operator/(const Vector& first, double k) {
	return Vector(first.x / k, first.y / k);
}

Vector operator*(double k, const Vector& first) {
	return first * k;
}

Vector operator/(double k, const Vector& first) {
	return first / k;
}

double operator*(const Vector& first, const Vector& second) {
	return first.x * second.x + first.y * second.y;
}

double operator/(const Vector& first, const Vector& second) {
	return first.x * second.y - first.y * second.x;
}

double Vector::operator()() const {
	return std::sqrt(x * x + y * y);
}

double Vector::min() const {
	return std::min(x, y);
}

double Vector::max() const {
	return std::max(x, y);
}

double Vector::absmin() const {
	return std::min(std::abs(x), std::abs(y));
}

double Vector::absmax() const {
	return std::max(std::abs(x), std::abs(y));
}

bool Vector::zero() const {
	return x == 0 && y == 0;
}

std::ostream& operator<<(std::ostream& out, const Vector& first) {
	out << first.x << " " << first.y;
	return out;
}

Vector Vector::makeUnit() const {
	return Vector(*this) / (*this)();
}


template <typename T>
int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

double ScalarProduct(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
	return (b - a) * (d - c);
}

double CrossProduct(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
	return (b - a) / (d - c);
}

int SignCrossProduct(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
	return sign((b - a) / (d - c));
}

bool SegmentIntersection(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
	return
		SignCrossProduct(a, b, a, c) * SignCrossProduct(a, b, a, d) <= 0 && 
		SignCrossProduct(c, d, c, a) * SignCrossProduct(c, d, c, b) <= 0;
}

bool PointInAABB(const Vector& a, const Vector& b, const Vector& point) {
	return point.x >= a.x && point.x <= b.x && point.y >= a.y && point.y <= b.y;
}


bool Polygon::IsConvex() const {
	for (size_t i = 0; i < Vertexes.size(); i++) {
		int j = (i + 1) % Vertexes.size();
		int k = (i + 2) % Vertexes.size();
		int cursgn = SignCrossProduct(Vertexes[i], Vertexes[j], Vertexes[j], Vertexes[k]);
		if (cursgn < 0) {
			return false;
		}
	}
	return true;
}

bool Polygon::IntersectPoint(const Vector& point) const {
	for (size_t i = 0; i < Vertexes.size(); i++) {
		int j = (i + 1) % Vertexes.size();
		int cursgn = SignCrossProduct(Vertexes[i], Vertexes[j], Vertexes[i], point);
		if (cursgn < 0) {
			return false;
		}
	}
	return true;
}

bool Polygon::IntersectSegment(const Vector& a, const Vector& b) const {
	for (size_t i = 0; i < Vertexes.size(); i++) {
		int j = (i + 1) % Vertexes.size();
		if (SegmentIntersection(Vertexes[i], Vertexes[j], a, b)) {
			return true;
		}
	}
	return false;
}

bool Polygon::IntersectAABB(const Vector& a, const Vector& b) const {
	if (Vertexes.empty()) {
		return false;
	}

	Vector tx = Vector(b.x - a.x, 0);
	Vector ty = Vector(0, b.y - a.y);
	return PointInAABB(a, b, Vertexes[0])
	 || IntersectSegment(a, a + tx)
	 || IntersectSegment(a, a + ty)
	 || IntersectSegment(b - tx, b)
	 || IntersectSegment(b - ty, b);
	
	return false;
}


bool Scene::IntersectPoint(const Vector& point) const {
	for (const auto& i : Objects) {
		if (i.IntersectPoint(point)) {
			return true;
		}
	}
	return false;
}

bool Scene::IntersectSegment(const Vector& a, const Vector& b) const {
	for (const auto& i : Objects) {
		if (i.IntersectSegment(a, b)) {
			return true;
		}
	}
	return false;
}

bool Scene::IntersectAABB(const Vector& a, const Vector& b) const {
	for (const auto& i : Objects) {
		if (i.IntersectAABB(a, b)) {
			return true;
		}
	}
	return false;
}


} // END namespace Geometry
