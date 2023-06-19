#pragma once
#include "TownMap.h"

class TGenCanvas {
private:
	TownMap* TMap = nullptr;

	bool E_pressed = false;
	Intersection* E_tmp = nullptr;
public:
	
	TGenCanvas(TownMap* TMap_);
	TGenCanvas();

	void drawDefault();
	void drawEdit();
	void drawManual();
	void drawDebug();

private:
	
	void drawConnectionDebug();
	void drawPlotsDebug();

	void drawPlotCenters();
	void drawIntersectionsAsPoints();

	void drawRiver();
	void drawOutsideConnections();
	void drawWalls();

	void drawInfrostructure();


	
	//void drawRoads();

	void listenManual();
	void listenEdit();

};