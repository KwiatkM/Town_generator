#pragma once
#include "utility.h"
#include "Intersection.h"

#include<vector>
#include<raylib.h>

#define BORDER_SCALE 0.9

enum plot_type {
	UNDEFINED_P,
	OMITTED,
	VALID
};

class Plot {
public:
	plot_type type = VALID;
	int center_id;
	std::vector<int> adj_intersection_id; // zmieniæ nazwe
	std::vector<int> neighbour_id;

	std::vector<Intersection*> adj_intersections; // 
	std::vector<Plot*> neighbours;

	std::vector<point_t> actual_border;
	Color c = { (unsigned char)(rand() % 255),(unsigned char)(rand() % 255),(unsigned char)(rand() % 255),0xFF };
public:
	Plot();
	Plot(int _center_id);
	bool hasIntersection(int intersection_id);
	void calculateActualBorders(std::vector<Intersection>& intersections, point_t center);
};