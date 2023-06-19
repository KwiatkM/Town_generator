#include "TownMap.h"

#include<random>
#include<queue>
#include<iostream>
#define LOG(x) std::cout << x << '\n'

TownMap::TownMap(int _n_points, int _canvas_width, int _canvas_height) {

	canvas_width = _canvas_width;
	canvas_height = _canvas_height;

}


void TownMap::generateFromExistingPoints()
{
	addOutsidePoints();
	triangulate();
	createPlots();
	calculateBorders();
}

void TownMap::generate() {
	generatePlotCenters_Grid();
	addOutsidePoints();
	triangulate();
	createPlots();
	menagePlotAndIntersectionsTypes();
	calculateBorders();
}

void TownMap::generatePlotCenters() {
	//int ofset = 10; //odleg³oœæ poza ekranem w której mog¹ wygenerowaæ siê punkty
	plot_centers.clear();



	if (options.plot_number < 10) 
	{
		options.plot_number = 10;
	}
	std::default_random_engine generator;
	if (options.seed < 0) {
		generator.seed(time(NULL));
	}
	else {
		generator.seed(options.seed);
	}
	
	std::uniform_int_distribution<int> distr_x(-ofset, canvas_width + ofset);
	std::uniform_int_distribution<int> distr_y(-ofset, canvas_height + ofset);

	for (int i = 0; i < options.plot_number; i++) {
		plot_centers.push_back({ distr_x(generator), distr_y(generator) });
	}
	//addOutsidePoints();
}

void TownMap::generatePlotCenters_Grid()
{
	plot_centers.clear();

	if (options.plot_number < 10)
	{
		options.plot_number = 10;
	}
	std::default_random_engine generator;
	if (options.seed < 0) {
		generator.seed(time(NULL));
	}
	else {
		generator.seed(options.seed);
	}

	double ratio = (double)canvas_width / (double)canvas_height;
	int nOfYPoints = round(sqrt(options.plot_number / ratio));
	int nOfXPoints = ratio * nOfYPoints;

	double rowSize = (double)canvas_height / (nOfYPoints - 1);
	double columnSize = (double)canvas_width / (nOfXPoints - 1);

	double maxOffset = canvas_height > canvas_width ? canvas_height : canvas_width;
	maxOffset *= 0.10;

	std::uniform_real_distribution<double> distr(0,1);
	for (int y = 0; y < nOfYPoints; y++) {
		for (int x = 0; x < nOfXPoints; x++) {
			plot_centers.push_back({(int)((columnSize*x) + (distr(generator) * maxOffset)),
									(int)((rowSize*y) + (distr(generator) * maxOffset))});
		}
	}
}

Intersection* TownMap::createRiver()
{
	Intersection* start = getRandomIntersection_Position(I_OUTSIDE,10);
	Intersection* end;
	if (options.coast) {
		end = gerRandomOceanIntersection();
	}
	else {
		end = getFurthestOutsideIntersection(start);
	}
	if (start == nullptr || end == nullptr) return nullptr;

	start->previousIntersection = start;
	std::queue<Intersection*> queue;
	queue.push(start);

	while (queue.size() != 0) {
		Intersection* i = queue.front();
		queue.pop();
		for (Intersection* c : i->connected_intersections) {
			if (c->previousIntersection == nullptr) {
				c->previousIntersection = i;
				queue.push(c);
			}
		}
	}

	Intersection* tmp = end;
	while (tmp != start) {
		tmp->isRiver = true;
		tmp = tmp->previousIntersection;
	}
	start->isRiver = true;
	return end;
}

void TownMap::createOutsideConnections(int n)
{
	outsideConnections.clear();
	for (int i = 0; i < n; i++) {

		Intersection* start = getRandomEdgeIntersection(i);
		start->tmp = start;
		Intersection* end = nullptr;
		std::queue<Intersection*> queue;
		queue.push(start);

		while (queue.size() != 0) {
			Intersection* i = queue.front();
			queue.pop();
			if (i->isTown) {
				end = i;
				break;
			}
			for (Intersection* c : i->connected_intersections) {
				if (c->tmp == nullptr && !c->isOcean) {
					c->tmp = i;
					queue.push(c);
				}
			}
		}
		if (end == nullptr) break;
		
		outsideConnections.push_back(std::vector<Intersection*>());
		Intersection* inter = end;
		while (inter->tmp != inter) {
			inter->isOutisdeRoad = true;
			outsideConnections[i].push_back(inter);
			inter = inter->tmp;
		}
		for (Intersection& i : intersections) {
			i.tmp = nullptr;
		}
		
	}

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

	/*plot_centers.push_back({ -200, -200 });
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
	plot_centers.push_back({ -200, (canvas_height + 400)  / 3 - 200 });*/
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

Plot* TownMap::getRandomPlot(plot_type type)
{
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_int_distribution<int> distr(0, plots.size() - 1);
	for (int i = 0; i < 500; i++) {
		int n = distr(generator);
		if (plots[n].type == type) {
			return &plots[n];
		}
	}
	return nullptr;
}

Plot* TownMap::getRandomFarmlandStartingPoint()
{
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_int_distribution<int> distr(0, plots.size() - 1);
	for (int i = 0; i < 500; i++) {
		int n = distr(generator);
		if (plots[n].type == P_UNUSED && plots[n].hasResidentialAsNeighbour()) {
			return &plots[n];
		}
	}
	return nullptr;
}

Intersection* TownMap::getRandomIntersection_Position(intersection_position pos,int seed)
{
	std::default_random_engine generator;
	generator.seed(seed);
	std::uniform_int_distribution<int> distr(0, intersections.size() - 1);

	for (int i = 0; i < 500; i++) {
		int n = distr(generator);
		if (intersections[n].position == pos && !intersections[n].isOcean) {
			return &intersections[n];
		}
	}
	return nullptr;
}

Intersection* TownMap::getRandomUnusedIntersection_Position(intersection_position pos)
{
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_int_distribution<int> distr(0, intersections.size() - 1);

	for (int i = 0; i < 500; i++) {
		int n = distr(generator);
		if (intersections[n].position == pos && !intersections[n].isOcean && !intersections[n].isTown && !intersections[n].isRiver && !intersections[n].isOutisdeRoad) {
			return &intersections[n];
		}
	}
	return nullptr;
}

Intersection* TownMap::gerRandomOceanIntersection()
{
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_int_distribution<int> distr(0, intersections.size() - 1);

	for (int i = 0; i < 500; i++) {
		int n = distr(generator);
		if (intersections[n].isOcean) {
			return &intersections[n];
		}
	}
	return nullptr;
}

Intersection* TownMap::getFurthestOutsideIntersection(Intersection* inter)
{
	point_t src = inter->coords;
	double maxDistance2 = 0;
	Intersection* furthest = nullptr;
	for (Intersection& i : intersections) {
		if (i.position == I_OUTSIDE) {
			double distance2 = dist2(src, i.coords);
			if (distance2 > maxDistance2) {
				maxDistance2 = distance2;
				furthest = &i;
			}
		}
	}
	return furthest;
}

Intersection* TownMap::getRandomEdgeIntersection(int seed)
{
	std::default_random_engine generator;
	generator.seed(seed);
	std::uniform_int_distribution<int> distr(0, intersections.size() - 1);

	for (int i = 0; i < 500; i++) {
		int n = distr(generator);
		if (intersections[n].connected_intersections.size() ==2 && !intersections[n].isOutisdeRoad) {
			return &intersections[n];
		}
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
		plots[i].type = P_RESIDENTIAL;
		bool hasOutsideIntersections = false;
		bool everyIntersectionOutside = true;

		//przyporz¹dkowywanie skrzy¿owañ przyleg³ych do dzia³ek //intersections.size == triangles.size
		for (int j = 0; j < intersections.size(); j++) {
			if (triangles.at(j).hasPoint(&plot_centers[i])) {
				
				plots[i].adj_intersections.push_back(&intersections[j]);
				intersections[j].adj_plots.push_back(&plots[i]);

				//
				if (intersections[j].position == I_OUTSIDE) {
					plots[i].type = P_UNUSED;
					hasOutsideIntersections = true;
				}
				else {
					everyIntersectionOutside = false;
				}
				
			}
		}
		if (plots[i].adj_intersections.size() <= 3 && !hasOutsideIntersections) {
			plots[i].type = P_NONE;
		}
		//oznaczenie skrzy¿owañ w mieœcie
		else if (!hasOutsideIntersections) {
			for (Intersection *i : plots[i].adj_intersections) {
				i->isTown = true;
			}
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

	//oznaczanie skrzy¿owañ z jednym po³¹czeniem
	for (Intersection& i : intersections) {
		if (i.connected_intersections.size() == 1) i.position = I_ON_EDGE;
	}
	
}



void TownMap::menagePlotAndIntersectionsTypes()
{
	//ocean
	if (options.coast) {
		getRandomPlot(P_UNUSED)->changeToOcean(0,0.4);
	}
	//farmlands
	int numberOfFarmland = (int)(sqrt(options.plot_number) * 0.5);
	int counter = 0;
	for (int i = 0; i < numberOfFarmland; i++) {
		Plot* p = getRandomFarmlandStartingPoint();
		if (p == nullptr) break;
		p->changeToFarmland(0, 0.2);
	}
	//town square
	if (options.town_square) {
		getRandomPlot(P_RESIDENTIAL)->type = P_TOWN_SQUARE;
	}

	//rest
	for (Plot& p : plots) {
		if (p.type == P_UNUSED) {
			if (p.hasMostlyOceanAsNeighbour()) {
				p.changeToOcean(0, 0.0);
			}
			else {
				p.type = P_FOREST;
			}
		}
	}

	//outsideConnections
	createOutsideConnections(4);

	//river
	if (options.river) {
		riverEnd = createRiver();
	}

	//walls

	
}



void TownMap::calculateBorders() {
	for (int i = 0; i < plot_centers.size(); i++) { 
		

		if (plots[i].type == P_RESIDENTIAL || plots[i].type == P_TOWN_SQUARE || plots[i].type == P_CHURCH) {
			plots[i].sortAdjIntersections(); //dzia³a tylko dla tych bo dla reszty skrzy¿owania nie zawsze ³¹cz¹ siê w cykl
			plots[i].calculateActualSortedBorders();

			if (plots[i].type == P_RESIDENTIAL) {
				plots[i].createBuildings();
			}
			
		}
		else {
			plots[i].calculateActualBoredr();
		}
	}
}

void TownMap::resizeCanvas(int canvas_width_, int canvas_height_) {
	canvas_width = canvas_width_;
	canvas_height = canvas_height_;
}

void TownMap::clear()
{
	roads.clear();
	intersections.clear();
	plots.clear();
	triangles.clear();
	plot_centers.clear();
}

