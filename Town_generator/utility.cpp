#include "utility.h"


point_t newPointFromRatio(point_t p1, point_t p2, double ratio) {
	return { p1.x - (int)((p1.x - p2.x) * ratio), p1.y - (int)((p1.y - p2.y) * ratio) };
}

double dist2(point_t p1, point_t p2) {
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

double dist2V(Vector2 v1, Vector2 v2) {
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}