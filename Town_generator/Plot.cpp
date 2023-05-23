#include "Plot.h"
#include <random>


Plot::Plot() {
	center_id = -1;
}

Plot::Plot(int _center_id) {
	center_id = _center_id;
}

bool Plot::hasIntersection(int intersecion_id) {
	for (int i : adj_intersection_id) {
		if (intersecion_id == i) return true;
	}
	return false;
}

void Plot::calculateActualBorders(std::vector<Intersection>& intersections, point_t center) {
	if (adj_intersection_id.size() > 0) {


		actual_border.clear();
		point_t avg_center = { 0,0 };
		for (int i = 0; i < adj_intersection_id.size(); i++) {
			avg_center.x += intersections[adj_intersection_id[i]].x;
			avg_center.y += intersections[adj_intersection_id[i]].y;
		}
		avg_center.x /= adj_intersection_id.size();
		avg_center.y /= adj_intersection_id.size();

		for (int i = 0; i < adj_intersection_id.size(); i++) {
			float dx = avg_center.x - intersections[adj_intersection_id[i]].x;
			float dy = avg_center.y - intersections[adj_intersection_id[i]].y;
			float scaled_dx = dx * BORDER_SCALE;
			float scaled_dy = dy * BORDER_SCALE;


			actual_border.push_back({ (int)(avg_center.x - scaled_dx) , (int)(avg_center.y - scaled_dy) });
		}
	}
}

//void Plot::calculateActualBorders(std::vector<Intersection>& intersections, point_t center) {
//	actual_border.clear();
//	for (int i = 0; i < adj_intersection_id.size(); i++) {
//		float dx = center.x - intersections[adj_intersection_id[i]].x;
//		float dy = center.y - intersections[adj_intersection_id[i]].y;
//		float scaled_dx = dx * BORDER_SCALE;
//		float scaled_dy = dy * BORDER_SCALE;
//		//if (scaled_dx < 3.0) scaled_dx = 3.0;  <---poprawiæ
//		//if (scaled_dy < 3.0) scaled_dy = 3.0;
//
//		actual_border.push_back({ (int)(center.x - scaled_dx) , (int)(center.y - scaled_dy) });
//	}
//}
