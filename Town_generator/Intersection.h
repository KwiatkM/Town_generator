#pragma once
#include "utility.h"
#include "Triangle.h"
#include <vector>

enum intersection_type {
	ROAD,
	WALL,
	RIVER
};

enum intersection_position {
	I_INSIDE,
	I_OUTSIDE,
	I_ON_BORDER
};

class Intersection {
public:
	point_t coords;
	Triangle* origin_triangle = nullptr;
	intersection_type type = ROAD;
	intersection_position position = I_INSIDE;
	std::vector<Intersection*> connected_intersections;
	
	Intersection(point_t _coords);
	Intersection(point_t _coords, intersection_type _type);
	
};

