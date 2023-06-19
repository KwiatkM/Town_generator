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
	std::vector<std::vector<Intersection*>> outsideConnections;



	int canvas_width;
	int canvas_height;
private:
	const int ofset = 10;
	const int border_thickness = -60;

	

public:
	mapOptions options = {};
	Intersection* riverEnd = nullptr;
	

public:
	TownMap(int _n_points, int _canvas_width, int _canvas_height);

	void generate();
	//void generate(int number_of_points, int seed);
	void generateFromExistingPoints();
	void generatePlotCenters();
	void generatePlotCenters_Grid();
	void triangulate();
	void createPlots();
	void menagePlotAndIntersectionsTypes();
	void calculateBorders();

	void resizeCanvas(int canvas_width_, int canvas_height_);
	void clear();

private:
	

	Intersection* createRiver();
	void createOutsideConnections(int n);

	void addOutsidePoints();
	Plot* findPlotWithCenter(point_t* center);
	Plot* getRandomPlot(plot_type type);
	Plot* getRandomFarmlandStartingPoint();

	Intersection* getRandomIntersection_Position(intersection_position pos, int seed);
	Intersection* getRandomUnusedIntersection_Position(intersection_position pos);
	Intersection* gerRandomOceanIntersection();
	Intersection* getFurthestOutsideIntersection(Intersection* inter);
	Intersection* getRandomEdgeIntersection(int seed);

};
