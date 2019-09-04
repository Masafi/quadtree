#pragma once
#include <ostream>
#include <vector>

namespace Geometry {

class Vector {
public:
	double x;
	double y;

	Vector();
	Vector(double x, double y);

	friend Vector operator+(const Vector& first, const Vector& second);
	friend Vector operator-(const Vector& first, const Vector& second);
	friend Vector operator*(const Vector& first, double k);
	friend Vector operator/(const Vector& first, double k);
	friend Vector operator*(double k, const Vector& first);
	friend Vector operator/(double k, const Vector& first);
	friend double operator*(const Vector& first, const Vector& second);
	friend double operator/(const Vector& first, const Vector& second);
	double operator()() const;
	friend std::ostream& operator<<(std::ostream& out, const Vector& first);

	double min() const;
	double max() const;
	double absmin() const;
	double absmax() const;

	bool zero() const;

	Vector makeUnit() const;
};

// Vectors AB and CD
double ScalarProduct(const Vector& a, const Vector& b, const Vector& c, const Vector& d);
double CrossProduct(const Vector& a, const Vector& b, const Vector& c, const Vector& d);
int SignCrossProduct(const Vector& a, const Vector& b, const Vector& c, const Vector& d);
bool SegmentIntersection(const Vector& a, const Vector& b, const Vector& c, const Vector& d);
// a - min coords point, b - max coords point
bool PointInAABB(const Vector& a, const Vector& b, const Vector& point);

// Convex
class Polygon {
public:
	std::vector<Vector> Vertexes;

	bool IsConvex() const;

	bool IntersectPoint(const Vector& point) const;

	bool IntersectSegment(const Vector& a, const Vector& b) const;

	bool IntersectAABB(const Vector& a, const Vector& b) const;
};

class Scene {
public:
	std::vector<Polygon> Objects;

	bool IntersectPoint(const Vector& point) const;

	bool IntersectAABB(const Vector& a, const Vector& b) const;

	bool IntersectSegment(const Vector& a, const Vector& b) const;
};

} // END namespace Geometry
