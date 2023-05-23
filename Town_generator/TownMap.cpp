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
	//boundPlots();
	//fixPlots();
	calculateBorders();
}

void TownMap::generate() {
	addOutsidePoints();
	triangulate();
	createPlots();
	//boundPlots();
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
		int p1id;
		int p2id;
		bool duplicate = false;

		bool equal(edge e) {
			return ((e.p1id == p1id) && (e.p2id == p2id))
				|| ((e.p2id == p1id) && (e.p1id == p2id));
		}
	};

	triangles.clear();
	std::vector<edge> edgeList;
	
	triangles.push_back({ -1,-2,-3, plot_centers });

	
	for (int i = 0; i < plot_centers.size(); i++) {
		for (int j = 0; j < triangles.size(); j++) {


			
			if (triangles[j].circumcircleContains(plot_centers[i])) {

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
			triangles.push_back(Triangle( edgeList.at(k).p1id, edgeList.at(k).p2id, i, plot_centers));
		}
		edgeList.clear();
	}
	
	//usuwanie trójk¹tów pomocniczych
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles.at(i).hasPoint(-1) || triangles.at(i).hasPoint(-2) || triangles.at(i).hasPoint(-3)) {
			triangles.erase(triangles.begin() + i);
			i--;
		}
	}

}

void TownMap::createPlots() {
	

	intersections.clear();
	plots.clear();
	roads.clear();

	for (int i = 0; i < triangles.size(); i++) {

		//tworzenie skrzy¿owañ
		intersections.push_back({	triangles.at(i).circumcircle.center.x,
									triangles.at(i).circumcircle.center.y });
		//je¿eli skrzy¿owanie poza granicami miasta -> oznacz
		if (intersections[i].x < -border_thickness ||
			intersections[i].x > canvas_width + border_thickness ||
			intersections[i].y < -border_thickness ||
			intersections[i].y > canvas_height + border_thickness) 
			{
			intersections[i].type = OUT_OF_BOUNDS;
			}
	}

	//tworzenie dróg (do rysowania)
	//³¹czenie skrzy¿owañ 
	for (int i = 0; i < triangles.size(); i++){
		for (int j = i + 1; j < triangles.size(); j++) {
			if (triangles[i].isAdjacent(triangles[j])) {
				roads.push_back({ i,j });
				intersections[j].connected_intersections_id.push_back(i);
				intersections[i].connected_intersections_id.push_back(j);

				//po dodaniu wskaŸników
				intersections[j].connected_intersections.push_back(&intersections[i]);
				intersections[i].connected_intersections.push_back(&intersections[j]);
				//
			}
		}
	}

	for (int i = 0; i < plot_centers.size(); i++) {
		plots.push_back({ i });
		
		//przyporz¹dkowywanie skrzy¿owañ przyleg³ych do dzia³ek
		for (int j = 0; j < intersections.size(); j++) {
			if (triangles.at(j).hasPoint(i)) {
				plots[i].adj_intersection_id.push_back(j);
				//po dodaniu wskaŸników
				plots[i].adj_intersections.push_back(&intersections[j]);
				//
				if (intersections[j].type == OUT_OF_BOUNDS) {
					plots[i].type = OMITTED;
				}
				
			}
		}
		if (plots[i].adj_intersection_id.size() <= 3) {
			plots[i].type = OMITTED;
		}
		
	}
	
}

void fixConnectionsOfNewIntersection(int inter1, int inter2, std::vector<Intersection>& intersections) {
	intersections.back().connected_intersections_id.push_back(inter1);
	intersections.back().connected_intersections_id.push_back(inter2);
	for (int k = 0; k < intersections[inter1].connected_intersections_id.size(); k++) {
		if (intersections[inter1].connected_intersections_id[k] == inter2) {
			intersections[inter1].connected_intersections_id[k] = intersections.size() - 1;
			break;
		}
	}
	for (int k = 0; k < intersections[inter2].connected_intersections_id.size(); k++) {
		if (intersections[inter2].connected_intersections_id[k] == inter1) {
			intersections[inter2].connected_intersections_id[k] = intersections.size() - 1;
			break;
		}
	}
}

void TownMap::boundPlots() {
	
	
	for (int i = 0; i < intersections.size(); i++) {

		bool abandon = true;
		for (int j = 0; j < intersections[i].connected_intersections_id.size(); j++) {
			if (intersections[intersections[i].connected_intersections_id[j]].type != OUT_OF_BOUNDS) {
				abandon = false;
				break;
			}
		}
		if (abandon) continue;


		//za bardzo w lewo
		if (intersections[i].x < -border_thickness) {

			for (int j = 0; j < intersections[i].connected_intersections_id.size(); j++) {
				int connected_intersection_id = intersections[i].connected_intersections_id[j];
				if (intersections[connected_intersection_id].type == OUT_OF_BOUNDS) continue;
				Vector2 new_intersection = { 500,0};
				
				if (!CheckCollisionLines({ (float)intersections[i].x, (float)intersections[i].y },
					{ (float)intersections[connected_intersection_id].x, (float)intersections[connected_intersection_id].y }, //XD a mog³em to robiæ na wskaŸnikach
					{ -(float)border_thickness, -(float)border_thickness },
					{ -(float)border_thickness, (float)canvas_height+border_thickness },
					&new_intersection)) {
					break;
				}
				

				// tworzenie nowych po³¹czeñ miêdzy skrzy¿owaniami
				intersections.push_back({ (int)new_intersection.x, (int)new_intersection.y, ON_BORDER });
				//intersections.back().connected_intersections_id.push_back(connected_intersection_id);
				//intersections.back().connected_intersections_id.push_back(i);

				fixConnectionsOfNewIntersection(connected_intersection_id, i, intersections);
				/*for (int k = 0; k < intersections[connected_intersection_id].connected_intersections_id.size(); k++) {
					if (intersections[connected_intersection_id].connected_intersections_id[k] == i) {
						intersections[connected_intersection_id].connected_intersections_id[k] = intersections.size() - 1;
						break;
					}
				}
				for (int k = 0; k < intersections[i].connected_intersections_id.size(); k++) {
					if (intersections[i].connected_intersections_id[k] == connected_intersection_id) {
						intersections[i].connected_intersections_id[k] = intersections.size() - 1;
						break;
					}
				}*/

			}
		}

		//za bardzo w prawo
		if (intersections[i].x > canvas_width + border_thickness) {
			for (int j = 0; j < intersections[i].connected_intersections_id.size(); j++) {
				int connected_intersection_id = intersections[i].connected_intersections_id[j];
				if (intersections[connected_intersection_id].type == OUT_OF_BOUNDS) continue;
				Vector2 new_intersection = { 0,0 };

				if (!CheckCollisionLines({ (float)intersections[i].x, (float)intersections[i].y },
					{ (float)intersections[connected_intersection_id].x, (float)intersections[connected_intersection_id].y }, //XD a mog³em to robiæ na wskaŸnikach
					{ (float)canvas_width + border_thickness, -(float)border_thickness },
					{ (float)canvas_width + border_thickness, (float)canvas_height + border_thickness },
					&new_intersection)) {
					break;
				}
					
				intersections.push_back({ (int)new_intersection.x, (int)new_intersection.y, ON_BORDER });
				fixConnectionsOfNewIntersection(connected_intersection_id, i, intersections);
			}

		}

		//za bardzo do góry
		if (intersections[i].y < -border_thickness) {
			for (int j = 0; j < intersections[i].connected_intersections_id.size(); j++) {
				int connected_intersection_id = intersections[i].connected_intersections_id[j];
				if (intersections[connected_intersection_id].type == OUT_OF_BOUNDS) continue;
				Vector2 new_intersection = { 0,0 };

				if (!CheckCollisionLines({ (float)intersections[i].x, (float)intersections[i].y },
					{ (float)intersections[connected_intersection_id].x, (float)intersections[connected_intersection_id].y }, //XD a mog³em to robiæ na wskaŸnikach
					{ -(float)border_thickness, -(float)border_thickness },
					{ (float)canvas_width + border_thickness, -(float)border_thickness },
					&new_intersection)) {
					break;
				}

				intersections.push_back({ (int)new_intersection.x, (int)new_intersection.y, ON_BORDER });
				fixConnectionsOfNewIntersection(connected_intersection_id, i, intersections);
			}
		}

		//za bardzo na dó³
		if (intersections[i].y > canvas_height + border_thickness) {
			for (int j = 0; j < intersections[i].connected_intersections_id.size(); j++) {
				int connected_intersection_id = intersections[i].connected_intersections_id[j];
				if (intersections[connected_intersection_id].type == OUT_OF_BOUNDS) continue;
				Vector2 new_intersection = { 0,0 };

				if (!CheckCollisionLines({ (float)intersections[i].x, (float)intersections[i].y },
					{ (float)intersections[connected_intersection_id].x, (float)intersections[connected_intersection_id].y }, //XD a mog³em to robiæ na wskaŸnikach
					{ -(float)border_thickness, (float)canvas_height + border_thickness },
					{ (float)canvas_width + border_thickness, (float)canvas_height + border_thickness },
					&new_intersection)) {
					break;
				}

				intersections.push_back({ (int)new_intersection.x, (int)new_intersection.y, ON_BORDER });
				fixConnectionsOfNewIntersection(connected_intersection_id, i, intersections);
			}
		}
	}

	/*

	for (int i = 0; i < intersections.size(); i++) {
		if (intersections[i].road_id.size() < 3) {

		}
	}*/
}

void TownMap::fixPlots() {
	for (Plot& p : plots) {
		for (int j = 0; j < p.adj_intersection_id.size(); j++) {
			if (intersections[p.adj_intersection_id[j]].type == OUT_OF_BOUNDS) {
				for (int c : intersections[p.adj_intersection_id[j]].connected_intersections_id) {
					if (intersections[c].type == ON_BORDER &&
						p.hasIntersection(intersections[c].connected_intersections_id[0])) {

						p.adj_intersection_id.push_back(c);
					}
				}
				p.adj_intersection_id.erase(p.adj_intersection_id.begin() + j);
				j--;
			}
		}
	}
	

}


void TownMap::calculateBorders() {
	for (int i = 0; i < plot_centers.size(); i++) {
		plots[i].calculateActualBorders(intersections, plot_centers[i]);
	}

}