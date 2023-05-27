#pragma once
#include "utility.h"
#include <vector>


class Triangle {
public:
	//int p1, p2, p3;
	point_t* p1, *p2, *p3;
	circle circumcircle;

	bool is_bad = false; //do algorytmu triangulacji

	Triangle(point_t* _p1, point_t* _p2, point_t* _p3);
	
	circle getCircumcircle();
	double getCircumcircleRadius2();
	point_t getCircumcircleCenter();

	void updateCircumcircle();
	bool isAdjacent(Triangle triangle);
	bool hasPoint(point_t *p);
	bool circumcircleContains(point_t *p);
};