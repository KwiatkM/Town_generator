#include "Intersection.h"

Intersection::Intersection(point_t _coords) {
	coords = _coords;
}

Intersection::Intersection(point_t _coords, intersection_type _type) {
	coords = _coords;
	//type = _type;
}

bool Intersection::hasConectionWith(Intersection* inter)
{
	for (Intersection* i : connected_intersections) {
		if (i == inter) return true;
	}
	return false;
}

double Intersection::getPlotInfluence()
{
	if (isRiver) {
		return 0.7;
	}
	if (isWall) {
		return 0.8;
	}
	if (isOutisdeRoad) {
		return 0.9;
	}
	if (isTown) {
		return 0.9;
	}
	return 1.0;
}



