#pragma once
#include <math.h>

struct point_t {
	int x;
	int y;

	point_t() : x{0} , y{0} {}
	point_t(int _x ,int _y) : x{_x}, y{_y} {}
};

struct elem_t {
	int value;
	elem_t* next = nullptr;
};

struct circle {
	point_t center;
	double r2;

	circle() : center{ point_t(0,0) } , r2{-1} {}
	circle(point_t _center, double _r) : center{ _center }, r2{ _r } {}
};


struct triangle {
	point_t p1, p2, p3;
	circle circumcircle;

	triangle(point_t _p1, point_t _p2, point_t _p3) :
		p1{ _p1 },
		p2{ _p2 },
		p3{ _p3 },
		circumcircle{}
	{
		const double t = ((p1.x * p1.x) + (p1.y * p1.y) - (p2.x * p2.x) - (p2.y * p2.y));
		const double u = ((p1.x * p1.x) + (p1.y * p1.y) - (p3.x * p3.x) - (p3.y * p3.y));
		const double j = ((p1.x - p2.x) * (p1.y - p3.y) - (p1.x - p3.x) * (p1.y - p2.y));

		const double circle_x = (-(p1.y - p2.y) * u + (p1.y - p3.y) * t) / (2 * j);
		const double circle_y = (-(p1.x - p2.x) * u - (p1.x - p3.x) * t) / (2 * j);

		circumcircle.center.x = circle_x;
		circumcircle.center.y = circle_y;
		circumcircle.r2 = sqrt((circle_x - p1.x)*(circle_x - p1.x) + (circle_y - p1.y)*(circle_y - p1.y));
	}

	
};

struct triangle_id {
	int id1, id2, id3;
	triangle tri;
	point_t* points;
	triangle_id(int canv_width, int canv_height) : // trójk¹t g³ówny
		id1{ -1 },	//lewy dolny
		id2{ -2 },	//górny
		id3{ -3 },	//prawy dolny
		tri{ point_t(-canv_height / 1.73, canv_height), point_t(canv_width / 2, canv_width * (-0.43)), point_t(canv_width + canv_height / 1.73, canv_height) },
		points{ nullptr }
	{}
	triangle_id(int _id1, int _id2, int _id3, point_t* _points) :
		id1{ _id1 },
		id2{ _id2 },
		id3{ _id3 },
		points{ _points },
		tri{ points[id1], points[id2], points[id3] }
	{}
};