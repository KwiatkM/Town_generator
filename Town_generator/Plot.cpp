#include "Plot.h"
#include <random>


Plot::Plot() {
	center = nullptr;
}

Plot::Plot(point_t* _center) {
	center = _center;
}

bool Plot::hasIntersection(Intersection* intersection) {
	for (Intersection* i : adj_intersections) {
		if (intersection == i) return true;
	}
	return false;
}

bool Plot::hasNeighbour(Plot* n) {
	for (Plot* p : neighbours) {
		if (n == p) return true;
	}
	return false;
}

bool Plot::hasNeighbourWithCenter(point_t* c) {
	for (Plot* p : neighbours) {
		if (p->center == c) return true;
	}
	return false;
}

void Plot::calculateActualBorders() {
	if (adj_intersections.size() > 0) {

		actual_border.clear();
		point_t avg_center = { 0,0 };
		for (int i = 0; i < adj_intersections.size(); i++) {
			avg_center.x += adj_intersections[i]->coords.x;
			avg_center.y += adj_intersections[i]->coords.y;
		}
		avg_center.x /= adj_intersections.size();
		avg_center.y /= adj_intersections.size();

		for (int i = 0; i < adj_intersections.size(); i++) {
			float dx = avg_center.x - adj_intersections[i]->coords.x;
			float dy = avg_center.y - adj_intersections[i]->coords.y;
			float scaled_dx = dx * BORDER_SCALE;
			float scaled_dy = dy * BORDER_SCALE;


			actual_border.push_back({ (int)(avg_center.x - scaled_dx) , (int)(avg_center.y - scaled_dy) });
		}
	}
}

