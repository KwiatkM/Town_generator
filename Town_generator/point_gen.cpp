
#include <raylib.h>
#include <random>
#include "point_gen.h"
#include "utility.h"

#define SEED 63	
#define NORMAL_DIST_STDDEV_RATIO 0.2f

void uniform_point_gen(int* x, int* y, int points_number, int max_x, int max_y, int seed = NULL) { // TODO dorobiæ seed
	std::default_random_engine generator(time(NULL)); // <- tu seed
	std::uniform_int_distribution<int> distr_x(0, max_x);
	std::uniform_int_distribution<int> distr_y(0, max_y);
	for (int i = 0; i < points_number; i++) {
		x[i] = distr_x(generator);
		y[i] = distr_y(generator);
	}
}

void normal_point_gen(int* x, int* y, int points_number, int max_x, int max_y) {
	std::default_random_engine generator(time(NULL));
	std::normal_distribution<> distr_x(max_x / 2.0f, NORMAL_DIST_STDDEV_RATIO * max_x);
	std::normal_distribution<> distr_y(max_y / 2.0f, NORMAL_DIST_STDDEV_RATIO * max_y);
	for (int i = 0; i < points_number; i++) {
		x[i] = (int)distr_x(generator);
		y[i] = (int)distr_y(generator);
	}
}

void uniform_point_gen(point_t *points, int points_number, int max_x, int max_y) {
	std::default_random_engine generator(time(NULL));
	std::uniform_int_distribution<int> distr_x(0, max_x);
	std::uniform_int_distribution<int> distr_y(0, max_y);
	for (int i = 0; i < points_number; i++) {
		points[i].x = distr_x(generator);
		points[i].y = distr_y(generator);
	}
}

void uniform_point_gen(std::vector<point_t> _plot_centers, int seed, int plots_number, int max_x, int max_y)
{
	_plot_centers.clear();
	if (seed < 0) {
		seed = NULL;
	}
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distr_x(0, max_x);
	std::uniform_int_distribution<int> distr_y(0, max_y);

	for(int i = 0; i < plots_number; i++){
		_plot_centers.push_back({ distr_x(generator), distr_y(generator) });
	}

}



void color_gen(Color* colors, int points_number) {
	srand(SEED);															
	for (int i = 0; i < points_number; i++) {
		colors[i] = { (unsigned char)(rand() % 255),(unsigned char)(rand() % 255),(unsigned char)(rand() % 255),0xFF };
	}
}