#include<raylib.h>
#include <vector>
#include "utility.h"
#ifndef POINT_GEN
#define POINT_GEN

void uniform_point_gen(int* x, int* y, int points_number, int max_x, int max_y);
void uniform_point_gen(point_t* points, int points_number, int max_x, int max_y);
void uniform_point_gen(std::vector<point_t> _plot_centers, int seed, int plots_number, int max_x, int max_y);

void normal_point_gen(int* x, int* y, int points_number, int max_x, int max_y);

void color_gen(Color* colors, int points_number);



#endif // !POINT_GEN

