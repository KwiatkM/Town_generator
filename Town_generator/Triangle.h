#pragma once
#include "utility.h"
#include <vector>


class Triangle {
public:
	int p1, p2, p3;
	circle circumcircle;
	bool is_bad = false;


	Triangle(int _p1, int _p2, int _p3);
	Triangle(int _p1, int _p2, int _p3, std::vector<point_t> plot_centers);
	circle getCircumcircle();
	void updateCircumcircle(std::vector<point_t> plot_centers);
	bool isAdjacent(Triangle triangle);
	bool hasPoint(int p);
	bool circumcircleContains(point_t p);
};