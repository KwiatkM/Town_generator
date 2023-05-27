#pragma once

#include <vector>

#include "utility.h"
#include "Plot.h"
#include "Triangle.h"
#include "Road.h"
#include "Intersection.h"


class TownMap {
public:

	std::vector<point_t> plot_centers;
	std::vector<Triangle> triangles;
	
	std::vector<Plot> plots;
	std::vector<Intersection> intersections;
	std::vector<Road> roads;
private:
	int canvas_width;
	int canvas_height;
	const int border_thickness = 50;

public:
	TownMap(int _n_points, int _canvas_width, int _canvas_height);
	void generate();
	void generate(int number_of_points, int seed);
	void generatePlotCenters(int number_of_points, int seed);
	void triangulate();
	void createPlots();
	void calculateBorders();

private:
	void addOutsidePoints();
	Plot* findPlotWithCenter(point_t* center);

};
