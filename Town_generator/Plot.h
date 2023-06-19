#pragma once
#include "utility.h"
#include "Intersection.h"
#include "Building.h"

#include<vector>
#include<raylib.h>



enum plot_type {
	P_NONE, //dzia³ka za ma³a
	P_UNUSED, //jeszcze nie przydzielony typ
	P_RESIDENTIAL,
	P_TOWN_SQUARE,
	P_CHURCH,
	P_FARMLAND,
	P_FOREST,
	P_WATER
};

class Plot {
public:
	//bool isValid = true;

	plot_type type = P_UNUSED;
	point_t* center;
	std::vector<Intersection*> adj_intersections; // 
	std::vector<Plot*> neighbours;

	std::vector<point_t> actual_border;

private:
	int distanceFromTownSquare = 9999;
	//tylko do rysowania dzia³ek
	std::vector<Intersection*> sortedAdjIntersections;

	point_t actualPlotCenter = { 0,0 }; //œrednia z actual_border
	double buildingSizeRatio = 0.5;
	int buildingsPerSide = 4;
	std::vector<Building> buildings;

public:
	Plot();
	Plot(point_t* _center);
	bool hasIntersection(Intersection* intersection);
	bool hasNeighbour(Plot* n);
	bool hasNeighbourWithCenter(point_t* c);
	void calculateActualSortedBorders();
	void sortAdjIntersections();
	void createBuildings();

	void calculateActualBoredr();
	
	void changeToOcean(int i, double chance);
	void changeToFarmland(int i, double chance);
	bool hasResidentialAsNeighbour();
	bool hasMostlyOceanAsNeighbour();
	bool hasNoOutsideIntersections();

	void draw();

private:
	void drawBuildings();
	void fill(Color color);
	void drawOcean();

};