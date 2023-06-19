#pragma once
#include <math.h>
#include <raylib.h>

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

enum townMapState {
	S_DEFAULT,
	S_EDIT_MODE,
	S_MANUAL_INPUT
};

struct mapOptions {

	int plot_number = 60; 
	int seed = -1;
	townMapState state = S_DEFAULT;

	bool debug = false;

	bool coast = false;
	bool river = false;
	bool town_square = false;
	bool church = false;
	bool walls = false;
};

point_t newPointFromRatio(point_t p1, point_t p2, double ratio);
double dist2(point_t p1, point_t p2);
double dist2V(Vector2 v1, Vector2 v2);


