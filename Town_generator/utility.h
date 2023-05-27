#pragma once
#include <math.h>

struct point_t {
	int x;
	int y;

	point_t() : x{0} , y{0} {}
	point_t(int _x ,int _y) : x{_x}, y{_y} {}

	bool equals(point_t p) {
		return ((x == p.x) && (y == p.y));
	}
};

struct circle {
	point_t center;
	double r2;

	circle() : center{ point_t(0,0) } , r2{-1} {}
	circle(point_t _center, double _r) : center{ _center }, r2{ _r } {}
};


