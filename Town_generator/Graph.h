#pragma once
#include <raylib.h>
#include <stdint.h>

#include "utility.h"

class Graph
{
private:
	uint16_t n_points = 0;
	point_t *points = NULL;
	elem_t **adj_list = NULL;

public:
	Graph();
	Graph(int n_points, point_t* points, elem_t **adj_list);
	Graph(int n_points, point_t* points);
	Graph(int n_points);

	void SetN_points(uint16_t n_points);

	point_t GetPoint(int point_nr);
	elem_t** getAdj_list();

	void generate_points(int max_x, int max_y);
	void add_conection(int point1, int point2); //DEBUG



	void free();
};



