#pragma once
#include<vector>
#include "utility.h"
#include <raylib.h>

#define BLD_COLOR {0xdd, 0x86, 0x6e, 0xff}
#define MIN_DRAW_WIDTH 10

class Building {
public:
	std::vector<point_t> boundaryPoints;//musi byæ posortowane
	
	void init();

private:
	double offsetRatio = 1;
	Vector2 center = { 0,0 };
	std::vector<Vector2> boundaryPointsScaled;

public:

	Building();
	Building(double offsetR);
	void draw();

private:
	void findCenter();
};