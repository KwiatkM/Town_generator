#pragma once
#include "utility.h"
#include "Triangle.h"
//#include "Plot.h"
#include <vector>
#include<raylib.h>



enum intersection_type {
	ROAD,
	WALL,
	RIVER,
	OCEAN
};

enum intersection_position {
	I_INSIDE,
	I_OUTSIDE,
	I_ON_EDGE
	
};

class Plot;
class Intersection {
public:
	point_t coords;
	Triangle* origin_triangle = nullptr;
	//intersection_type type = ROAD;
	bool isOutisdeRoad = false;
	bool isWall = false;
	bool isRiver = false;
	bool isTown = false;
	bool isOcean = false;

	int distanceFromTownSquare = 9999;
	Intersection* previousIntersection = nullptr; //do wyznaczenia rzek
	Intersection* tmp = nullptr;

	intersection_position position = I_INSIDE;
	std::vector<Plot*> adj_plots;
	std::vector<Intersection*> connected_intersections;
	
	Intersection(point_t _coords);
	Intersection(point_t _coords, intersection_type _type);
	bool hasConectionWith(Intersection* inter);
	double getPlotInfluence();

	//Color getColor();
	
};

