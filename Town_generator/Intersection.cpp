#include "Intersection.h"

Intersection::Intersection(int _x, int _y) {
	x = _x;
	y = _y;
}

Intersection::Intersection(int _x, int _y, intersection_type _type) {
	x = _x;
	y = _y;
	type = _type;
}

