#include "Intersection.h"

Intersection::Intersection(point_t _coords) {
	coords = _coords;
}

Intersection::Intersection(point_t _coords, intersection_type _type) {
	coords = _coords;
	type = _type;
}

