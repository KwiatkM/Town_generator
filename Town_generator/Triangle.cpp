#include "utility.h"
#include "Triangle.h"

Triangle::Triangle(point_t* _p1, point_t* _p2, point_t* _p3) {
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	updateCircumcircle();
}

circle Triangle::getCircumcircle() {
	return circumcircle;
}

double Triangle::getCircumcircleRadius2() {
	return circumcircle.r2;
}

point_t Triangle::getCircumcircleCenter() {
	return circumcircle.center;
}

void Triangle::updateCircumcircle() {
	
	// obliczanie okrêgu wpistanego w trójk¹t
	const double t = ((p1->x * p1->x) + (p1->y * p1->y) - (p2->x * p2->x) - (p2->y * p2->y));
	const double u = ((p1->x * p1->x) + (p1->y * p1->y) - (p3->x * p3->x) - (p3->y * p3->y));
	const double j = ((p1->x - p2->x) * (p1->y - p3->y) - (p1->x - p3->x) * (p1->y - p2->y));

	const double circle_x = (-(p1->y - p2->y) * u + (p1->y - p3->y) * t) / (2 * j);
	const double circle_y = ((p1->x - p2->x) * u - (p1->x - p3->x) * t) / (2 * j);

	circumcircle.center.x = circle_x;
	circumcircle.center.y = circle_y;
	circumcircle.r2 = (circle_x - p1->x) * (circle_x - p1->x) + (circle_y - p1->y) * (circle_y - p1->y);
}

bool Triangle::isAdjacent(Triangle triangle) {

	// xd
	if (p1 == triangle.p1 || p1 == triangle.p2 || p1 == triangle.p3) {
		if (p2 == triangle.p1 || p2 == triangle.p2 || p2 == triangle.p3 ||
			p3 == triangle.p1 || p3 == triangle.p2 || p3 == triangle.p3) {
			return true;
		}
	}
	if (p2 == triangle.p1 || p2 == triangle.p2 || p2 == triangle.p3) {
		if (p3 == triangle.p1 || p3 == triangle.p2 || p3 == triangle.p3) {
			return true;
		}
	}
	return false;
}

bool Triangle::hasPoint(point_t* p) {

	if (p1 == p) return true;
	if (p2 == p) return true;
	if (p3 == p) return true;
	return false;
}

bool Triangle::circumcircleContains(point_t *p) {
	int dx = p->x - circumcircle.center.x;
	int dy = p->y - circumcircle.center.y;
	return (dx * dx) + (dy * dy) < circumcircle.r2;
}