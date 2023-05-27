#pragma once
#include "Intersection.h"

class Road {
public:
	Intersection* intersection1, * intersection2;

	Road(Intersection* _intersection1, Intersection* _intersection2);
};