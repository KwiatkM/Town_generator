#include "Building.h"


void Building::init()
{
	findCenter();
	for (point_t &p : boundaryPoints) {
		point_t tmp =  {newPointFromRatio({(int)center.x, (int)center.y}, p, offsetRatio)};
		boundaryPointsScaled.push_back({ (float)tmp.x, (float)tmp.y });
	}
}

Building::Building()
{
	center = { 0,0 };
}

Building::Building(double offsetR)
{
	offsetRatio = offsetR;
}

void Building::draw()
{
	if (dist2V(boundaryPointsScaled.front(), boundaryPointsScaled.back()) < MIN_DRAW_WIDTH * MIN_DRAW_WIDTH) return;

	for (int i = 0; i < boundaryPointsScaled.size()-1; i++) {
		DrawTriangle(center, boundaryPointsScaled[i], boundaryPointsScaled[i + 1], BLD_COLOR);
		DrawTriangle(center, boundaryPointsScaled[i+1], boundaryPointsScaled[i], BLD_COLOR);
	}
	DrawTriangle(center, boundaryPointsScaled.front(), boundaryPointsScaled.back(), BLD_COLOR);
	DrawTriangle(center, boundaryPointsScaled.back(), boundaryPointsScaled.front(), BLD_COLOR);

	for (int i = 0; i < boundaryPointsScaled.size() - 1; i++) {
		DrawLineV(boundaryPointsScaled[i], boundaryPointsScaled[i + 1], BLACK);
	}
	DrawLineV(boundaryPointsScaled.back(), boundaryPointsScaled.front(), BLACK);

}

void Building::findCenter()
{
	float sum_x = 0, sum_y = 0;
	for (int i = 0; i < boundaryPoints.size(); i++) {
		sum_x += boundaryPoints[i].x;
		sum_y += boundaryPoints[i].y;
	}
	center = { sum_x / boundaryPoints.size() , sum_y / boundaryPoints.size() };
}
