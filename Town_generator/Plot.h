#pragma once
#include "utility.h"
#include "Intersection.h"

#include<vector>
#include<raylib.h>

#define BORDER_SCALE 0.9

enum plot_type {
	P_RESIDENTIAL,
	P_TOWN_SQUARE,
	P_CHURCH,
	P_FARMLAND,
	P_FOREST,
	P_WATER
};

class Plot {
public:
	bool isValid = true;

	plot_type type = P_RESIDENTIAL;
	point_t* center;
	std::vector<Intersection*> adj_intersections; // 
	std::vector<Plot*> neighbours;

	std::vector<point_t> actual_border;

public:
	Plot();
	Plot(point_t* _center);
	bool hasIntersection(Intersection* intersection);
	bool hasNeighbour(Plot* n);
	bool hasNeighbourWithCenter(point_t* c);
	void calculateActualBorders();
};