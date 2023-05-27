#include "TownMap.h"

#include<random>
#define LOG(x) std::cout << x << '\n'

TownMap::TownMap(int _n_points, int _canvas_width, int _canvas_height) {

	canvas_width = _canvas_width;
	canvas_height = _canvas_height;

}


void TownMap::generate(int number_of_points, int seed) {
	generatePlotCenters(number_of_points, seed);
	triangulate();
	createPlots();
	calculateBorders();
}

void TownMap::generate() {
	addOutsidePoints();
	triangulate();
	createPlots();
	calculateBorders();
}

void TownMap::generatePlotCenters(int plots_number, int seed) {
	int ofset = 10;
	plot_centers.clear();

	if (plots_number < 2) 
	{
		plots_number = 3;
	}
	std::default_random_engine generator;
	if (seed < 0) {
		generator.seed(time(NULL));
	}
	else {
		generator.seed(seed);
	}
	
	std::uniform_int_distribution<int> distr_x(-ofset, canvas_width + ofset);
	std::uniform_int_distribution<int> distr_y(-ofset, canvas_height + ofset);

	for (int i = 0; i < plots_number; i++) {
		plot_centers.push_back({ distr_x(generator), distr_y(generator) });
	}
	//addOutsidePoints();
}

void TownMap::addOutsidePoints() {
	plot_centers.push_back({ -50, -50 });
	plot_centers.push_back({ canvas_width + 50, -50 });
	plot_centers.push_back({ -50, canvas_height + 50 });
	plot_centers.push_back({ canvas_width + 50, canvas_height + 50 });

	plot_centers.push_back({ -50, canvas_height/2 + 25 });
	plot_centers.push_back({ canvas_width / 2 + 25, -50 });
	plot_centers.push_back({ canvas_width+50, canvas_height/2 + 25 });
	plot_centers.push_back({ canvas_width/2 + 25, canvas_height + 50 });

	plot_centers.push_back({ -200, -200 });
	plot_centers.push_back({ (canvas_width+400)/3 -200, -200 });
	plot_centers.push_back({ (canvas_width + 400) * 2 / 3 - 200, -200 });
	plot_centers.push_back({  canvas_width + 200, -200 });
	plot_centers.push_back({ canvas_width + 200, (canvas_height + 400)/3 -200});
	plot_centers.push_back({ canvas_width + 200, (canvas_height + 400)*2 / 3 - 200 });
	plot_centers.push_back({ canvas_width + 200, canvas_height + 200 });
	plot_centers.push_back({ (canvas_width + 400) * 2 / 3 - 200, canvas_height + 200 });
	plot_centers.push_back({ (canvas_width + 400) / 3 - 200, canvas_height + 200 });
	plot_centers.push_back({ -200, canvas_height + 200 });
	plot_centers.push_back({ -200, (canvas_height + 400) * 2 / 3 - 200 });
	plot_centers.push_back({ -200, (canvas_height + 400)  / 3 - 200 });
}

void TownMap::triangulate() {

	struct edge {
		point_t *p1;
		point_t *p2;
		bool duplicate = false;

		bool equal(edge e) {
			return ((e.p1 == p1) && (e.p2 == p2))
				|| ((e.p2 == p1) && (e.p1 == p2));
		}
	};

	triangles.clear();
	std::vector<edge> edgeList;
	
	point_t dp1 = { -4000,3200 };	//punkty pomocnicze
	point_t dp2 = { 2000,-4000 };
	point_t dp3 = { 4000,3200 };

	triangles.push_back({ &dp1, &dp2, &dp3 });

	
	for (int i = 0; i < plot_centers.size(); i++) {
		for (int j = 0; j < triangles.size(); j++) {
			
			if (triangles[j].circumcircleContains(&plot_centers[i])) {

				edgeList.push_back({ triangles[j].p1, triangles[j].p2 });
				edgeList.push_back({ triangles[j].p2, triangles[j].p3 });
				edgeList.push_back({ triangles[j].p1, triangles[j].p3 });
				triangles[j].is_bad = true;
			}
		}

		//usuwanie z³ych trójk¹tów
		triangles.erase(std::remove_if(begin(triangles),end(triangles), [](Triangle& t) {
			return t.is_bad;
			}), end(triangles));


		for (int k = 0; k < edgeList.size(); k++) { //znajdowanie identycznych krawêdzi
			for (int l = k+1; l < edgeList.size(); l++) {
				if (edgeList.at(k).equal(edgeList.at(l))) {

					edgeList.at(k).duplicate = true;
					edgeList.at(l).duplicate = true;
					break;
				}
			}
		}

		//usuwanie jednakowych krawêdzi
		edgeList.erase(std::remove_if(begin(edgeList), end(edgeList), [](edge& e) {
			return e.duplicate;
			}), end(edgeList));
		
		
		//dodawanie trójk¹tów
		for (int k = 0; k < edgeList.size(); k++) {
			triangles.push_back(Triangle( edgeList.at(k).p1, edgeList.at(k).p2, &plot_centers.at(i)));
		}
		edgeList.clear();
	}
	
	//usuwanie trójk¹tów pomocniczych
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles.at(i).hasPoint(&dp1) || triangles.at(i).hasPoint(&dp2) || triangles.at(i).hasPoint(&dp3)) {
			triangles.erase(triangles.begin() + i);
			i--;
		}
	}

}

Plot* TownMap::findPlotWithCenter(point_t* center) {
	for (Plot& p : plots) {
		if (p.center == center) return &p;
	}
	return nullptr;
}

void TownMap::createPlots() {
	

	intersections.clear();
	plots.clear();
	roads.clear();

	for (int i = 0; i < triangles.size(); i++) {

		//tworzenie skrzy¿owañ
		intersections.push_back({ {	triangles.at(i).getCircumcircleCenter().x,
									triangles.at(i).getCircumcircleCenter().y } });
		intersections.at(i).origin_triangle = &triangles.at(i);

		//je¿eli skrzy¿owanie poza granicami miasta -> oznacz
		if (intersections[i].coords.x < -border_thickness ||
			intersections[i].coords.x > canvas_width + border_thickness ||
			intersections[i].coords.y < -border_thickness ||
			intersections[i].coords.y > canvas_height + border_thickness)
			{
			intersections[i].position = I_OUTSIDE;
			}
	}

	//tworzenie dróg (do rysowania)
	//³¹czenie skrzy¿owañ 
	for (int i = 0; i < triangles.size(); i++){
		for (int j = i + 1; j < triangles.size(); j++) {
			if (triangles[i].isAdjacent(triangles[j])) {
				roads.push_back({ &intersections[i],&intersections[j]});
				
				intersections[j].connected_intersections.push_back(&intersections[i]);
				intersections[i].connected_intersections.push_back(&intersections[j]);
				
			}
		}
	}

	//tworzenie dzia³ek
	for (int i = 0; i < plot_centers.size(); i++) { //
		plots.push_back({ &plot_centers.at(i) });
		
		//przyporz¹dkowywanie skrzy¿owañ przyleg³ych do dzia³ek //intersections.size == triangles.size
		for (int j = 0; j < intersections.size(); j++) {
			if (triangles.at(j).hasPoint(&plot_centers[i])) {
				
				plots[i].adj_intersections.push_back(&intersections[j]);
				
				//
				if (intersections[j].position == I_OUTSIDE) {
					plots[i].isValid = false;
				}
				
			}
		}
		if (plots[i].adj_intersections.size() <= 3) {
			plots[i].isValid = false;
		}
	}

	//³¹czenie s¹siaduj¹cych dzia³ek
	for (Plot& p : plots) {
		for (Intersection* i : p.adj_intersections) {
			if (i->origin_triangle->p1 != p.center && !p.hasNeighbourWithCenter(i->origin_triangle->p1)) {
				p.neighbours.push_back(findPlotWithCenter(i->origin_triangle->p1));
			}
			if (i->origin_triangle->p2 != p.center && !p.hasNeighbourWithCenter(i->origin_triangle->p2)) {
				p.neighbours.push_back(findPlotWithCenter(i->origin_triangle->p2));
			}
			if (i->origin_triangle->p3 != p.center && !p.hasNeighbourWithCenter(i->origin_triangle->p3)) {
				p.neighbours.push_back(findPlotWithCenter(i->origin_triangle->p3));
			}
		}
	}
	
}

void TownMap::calculateBorders() {
	for (int i = 0; i < plot_centers.size(); i++) {
		plots[i].calculateActualBorders();
	}
}