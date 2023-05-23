#pragma once
#include <vector>

enum intersection_type {
	UNDEFINED,
	OUT_OF_BOUNDS,
	INSIDE_TOWN,
	ON_BORDER
};

class Intersection {
public:
	int x, y;
	intersection_type type = INSIDE_TOWN;
	std::vector<int> connected_intersections_id;
	
	std::vector<Intersection*> connected_intersections;
	


	Intersection(int _x, int _y);
	Intersection(int _x, int _y, intersection_type _type);
	
};

