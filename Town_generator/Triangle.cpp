#include "utility.h"
#include "Triangle.h"

Triangle::Triangle(int _p1, int _p2, int _p3) {
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	circumcircle = circle(); //??????
}

Triangle::Triangle(int _p1, int _p2, int _p3, std::vector<point_t> plot_centers) {
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	
	updateCircumcircle(plot_centers);
}

circle Triangle::getCircumcircle() {
	return circumcircle;
}

void Triangle::updateCircumcircle(std::vector<point_t> plot_centers) {
	int p1x, p1y, p2x, p2y, p3x, p3y;
	point_t bigTr[3] = { {-4000,3200}, {2000,-4000}, {4000,3200} };
	if (p1 < 0) {		//do zmiany
		p1x = bigTr[p1+3].x;
		p1y = bigTr[p1 + 3].y;
	}
	else {
		p1x = plot_centers[p1].x;
		p1y = plot_centers[p1].y;
	}

	if (p2 < 0) {		//do zmiany
		p2x = bigTr[p2 + 3].x;
		p2y = bigTr[p2 + 3].y;
	}
	else {
		p2x = plot_centers[p2].x;
		p2y = plot_centers[p2].y;
	}

	if (p3 < 0) {		//do zmiany
		p3x = bigTr[p3 + 3].x;
		p3y = bigTr[p3 + 3].y;
	}
	else {
		p3x = plot_centers[p3].x;
		p3y = plot_centers[p3].y;
	}

	// obliczanie okrêgu wpistanego w trójk¹t
	const double t = ((p1x * p1x) + (p1y * p1y) - (p2x * p2x) - (p2y * p2y));
	const double u = ((p1x * p1x) + (p1y * p1y) - (p3x * p3x) - (p3y * p3y));
	const double j = ((p1x - p2x) * (p1y - p3y) - (p1x - p3x) * (p1y - p2y));

	const double circle_x = (-(p1y - p2y) * u + (p1y - p3y) * t) / (2 * j);
	const double circle_y = ((p1x - p2x) * u - (p1x - p3x) * t) / (2 * j);

	circumcircle.center.x = circle_x;
	circumcircle.center.y = circle_y;
	circumcircle.r2 = (circle_x - p1x) * (circle_x - p1x) + (circle_y - p1y) * (circle_y - p1y);
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

bool Triangle::hasPoint(int p) {

	// xd * 2
	if (p1 == p) return true;
	if (p2 == p) return true;
	if (p3 == p) return true;
	return false;
}

bool Triangle::circumcircleContains(point_t p) {
	int dx = p.x - circumcircle.center.x;
	int dy = p.y - circumcircle.center.y;
	return (dx * dx) + (dy * dy) < circumcircle.r2;
}