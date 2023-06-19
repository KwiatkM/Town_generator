#include "Plot.h"
#include <random>
#include <algorithm>


Plot::Plot() {
	center = nullptr;
}

Plot::Plot(point_t* _center) {
	center = _center;
}

bool Plot::hasIntersection(Intersection* intersection) {
	for (Intersection* i : adj_intersections) {
		if (intersection == i) return true;
	}
	return false;
}

bool Plot::hasNeighbour(Plot* n) {
	for (Plot* p : neighbours) {
		if (n == p) return true;
	}
	return false;
}

bool Plot::hasNeighbourWithCenter(point_t* c) {
	for (Plot* p : neighbours) {
		if (p->center == c) return true;
	}
	return false;
}

void Plot::calculateActualSortedBorders() {

	if (sortedAdjIntersections.size() > 0) {
		actual_border.clear();

		if (type == P_RESIDENTIAL) {

			double borderScale = 0.0;
			point_t avg_center = { 0,0 };
			for (int i = 0; i < sortedAdjIntersections.size(); i++) {
				avg_center.x += sortedAdjIntersections[i]->coords.x;
				avg_center.y += sortedAdjIntersections[i]->coords.y;
			}
			avg_center.x /= sortedAdjIntersections.size();
			avg_center.y /= sortedAdjIntersections.size();

			for (int i = 0; i < sortedAdjIntersections.size(); i++) {
				borderScale = sortedAdjIntersections[i]->getPlotInfluence();
				float dx = avg_center.x - sortedAdjIntersections[i]->coords.x;
				float dy = avg_center.y - sortedAdjIntersections[i]->coords.y;
				float scaled_dx = dx * borderScale;
				float scaled_dy = dy * borderScale;


				actual_border.push_back({ (int)(avg_center.x - scaled_dx) , (int)(avg_center.y - scaled_dy) });
			}
		}
		else {
			point_t avg_center = { 0,0 };
			for (Intersection* i : sortedAdjIntersections) {
				avg_center.x += i->coords.x;
				avg_center.y += i->coords.y;
				actual_border.push_back({ i->coords.x , i->coords.y });
			}
			avg_center.x /= sortedAdjIntersections.size(); //dla np oceanu nie da siê pozortowaæ
			avg_center.y /= sortedAdjIntersections.size();
			actualPlotCenter = avg_center;
		}

	}
}

void Plot::sortAdjIntersections()
{
	sortedAdjIntersections.clear();

	Intersection* currentIntersection = adj_intersections[0];
	Intersection* previousIntersection = nullptr;
	sortedAdjIntersections.push_back(currentIntersection);

	while(adj_intersections.size() != sortedAdjIntersections.size()) {
		for (Intersection* i : adj_intersections) {
			if (currentIntersection->hasConectionWith(i) && previousIntersection != i) {
				previousIntersection = currentIntersection;
				currentIntersection = i;
				sortedAdjIntersections.push_back(i);
				break;
			}
		}
	}

}



void Plot::createBuildings()
{
	//trzeba zamieniæ na podstawie jakich punktów generowane s¹ budynki
	// przede wszystkim zamieniæ sortedAdjIntersections na coœ innego

	//wyznaczenie œrodka dzia³ki
	point_t avgPlotCenter = { 0,0 };
	for (point_t &p : actual_border) {
		avgPlotCenter.x += p.x;
		avgPlotCenter.y += p.y;
	}
	avgPlotCenter.x /= actual_border.size();
	avgPlotCenter.y /= actual_border.size();
	
	//liczenie obwodu dzia³ki
	double perimeterLength = 0.0;
	int perimeterSegmentsNumber = actual_border.size();
	//double* perimeterSegmentsLengths = new double[perimeterSegmentsNumber]; // p[0] = odleg³oœæ od 0 -> 1 skrzy¿owania
	std::vector<double> perimeterSegmentsLengthsV;
	
	for (int i = 0; i < actual_border.size()-1; i++) {
		double dist = sqrt(dist2( actual_border[i], actual_border[i + 1]));
		perimeterSegmentsLengthsV.push_back(dist);
		perimeterLength += dist;
	}
	/////////odleg³oœæ od ostatniego punktu do zerowego
	perimeterSegmentsLengthsV.push_back(sqrt(dist2(actual_border[0], actual_border.back())));
	perimeterLength += perimeterSegmentsLengthsV.back();


	double averagePerimeterSegmentLength = perimeterLength / perimeterSegmentsNumber;

	// ile punktów na segment
	//int* pointsPerSegments = new int[perimeterSegmentsNumber];
	std::vector<int> pointsPerSegmentsV;
	double slope = buildingsPerSide / averagePerimeterSegmentLength; //do funkcji przydzielaj¹cej iloœæ budynków
	int numberOfPointToGenerate = 0;
	for (int i = 0; i < perimeterSegmentsNumber; i++) {
		pointsPerSegmentsV.push_back(round(slope * perimeterSegmentsLengthsV.at(i)));
		numberOfPointToGenerate += pointsPerSegmentsV[i];
	}

	
	//generowanie punktów
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_real_distribution<double> distribution(0, 1);

	
	std::vector<point_t> buildingStartingPointsV;
	std::vector<double> bufV;
	for (int i = 0; i < perimeterSegmentsNumber-1; i++) {
		bufV.clear();
		if (pointsPerSegmentsV[i] != 0) {
			for (int j = 0; j < pointsPerSegmentsV[i]; j++) {
				bufV.push_back(distribution(generator));
			}
			std::sort(bufV.begin(), bufV.end()); //sprawdziæ czy poprawne
			for (int j = 0; j < pointsPerSegmentsV[i]; j++) {
				buildingStartingPointsV.push_back(newPointFromRatio(actual_border[i], actual_border[i + 1], bufV[j]));

			}
		}
	}
	// dla ostatniego skrzy¿owania (³¹czy siê z pierwszym)
	bufV.clear();
	if (pointsPerSegmentsV.back() != 0) {
		for (int j = 0; j < pointsPerSegmentsV.back(); j++) {
			bufV.push_back(distribution(generator));
		}
		std::sort(bufV.begin(), bufV.end()); //sprawdziæ czy poprawne
		for (int j = 0; j < pointsPerSegmentsV.back(); j++) {
			buildingStartingPointsV.push_back(newPointFromRatio(actual_border.back(), actual_border.front(), bufV[j]));

		}
	}
	

	//tworzenie budynków
	std::uniform_real_distribution<double> buildingOffsetRatio(0.90, 1.1);
	int currentPoint = 0;

	
	for (int i = 0; i < buildingStartingPointsV.size() - 1; i++) {
		Building b = { buildingOffsetRatio(generator) };
		b.boundaryPoints.push_back(buildingStartingPointsV[i]);
		b.boundaryPoints.push_back(newPointFromRatio(buildingStartingPointsV[i], avgPlotCenter, buildingSizeRatio));
		b.boundaryPoints.push_back(newPointFromRatio(buildingStartingPointsV[i + 1], avgPlotCenter, buildingSizeRatio));
		b.boundaryPoints.push_back(buildingStartingPointsV[i + 1]);
		buildings.push_back(b);
	}
	Building b = { buildingOffsetRatio(generator) };
	b.boundaryPoints.push_back(buildingStartingPointsV.back());
	b.boundaryPoints.push_back(newPointFromRatio(buildingStartingPointsV.back(), avgPlotCenter, buildingSizeRatio));
	b.boundaryPoints.push_back(newPointFromRatio(buildingStartingPointsV.front(), avgPlotCenter, buildingSizeRatio));
	b.boundaryPoints.push_back(buildingStartingPointsV.front());
	buildings.push_back(b);

	//kjh
	for (Building& b : buildings) {
		b.init();
	}
}

void Plot::calculateActualBoredr()
{
	actual_border.clear();
	point_t avgCenter = { 0,0 };
	for (Intersection* i : adj_intersections) {
		avgCenter.x += i->coords.x;
		avgCenter.y += i->coords.y;
	}
	avgCenter.x = (int)((float)avgCenter.x / adj_intersections.size());
	avgCenter.y = (int)((float)avgCenter.y / adj_intersections.size());
	actualPlotCenter = avgCenter;
	for (Intersection* i : adj_intersections) {
		double influence = i->getPlotInfluence();
		actual_border.push_back(newPointFromRatio(avgCenter,i->coords,influence));
	}
}

void Plot::changeToOcean(int i, double chance)
{
	type = P_WATER;
	for (Intersection* i : adj_intersections) {
		i->isOcean = true;
	}
	i++;
	if (i > 4) return;

	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_real_distribution<double> distribution(0, 1);

	for (Plot* p : neighbours) {
		if (p->type == P_UNUSED && distribution(generator) < chance) {
			p->changeToOcean(i, chance);
		}
	}
}

void Plot::changeToFarmland(int i, double chance)
{
	type = P_FARMLAND;
	i++;
	if (i > 2) return;

	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_real_distribution<double> distribution(0, 1);

	for (Plot* p : neighbours) {
		if (p->type == P_UNUSED && distribution(generator) < chance) {
			p->changeToFarmland(i, chance);
		}
	}
}

bool Plot::hasResidentialAsNeighbour()
{
	for (Plot* n : neighbours) {
		if (n->type == P_RESIDENTIAL) {
			return true;
		}
	}
	return false;
}

bool Plot::hasMostlyOceanAsNeighbour()
{
	int oceanCount = 0;
	for (Plot* n : neighbours) {
		if (n->type == P_WATER) oceanCount++;
	}
	if (((float)oceanCount / neighbours.size()) > 0.5) return true;
	return false;
}

bool Plot::hasNoOutsideIntersections()
{
	for (Intersection* i : adj_intersections) {
		if (i->position == I_OUTSIDE) return false;
	}
	return true;
}


void Plot::draw()
{
	switch (type)
	{
	case P_NONE:
		break;
	case P_RESIDENTIAL:
		drawBuildings();
		break;
	case P_TOWN_SQUARE:
		fill(LIGHTGRAY);
		break;
	case P_CHURCH:
		break;
	case P_FARMLAND:
		fill(YELLOW);
		break;
	case P_FOREST:
		fill(GREEN);
		break;
	case P_WATER:
		fill(BLUE);
		break;
	default:
		break;
	}
}

void Plot::drawBuildings()
{
	for (Building& b : buildings) {
		b.draw();
	}
}

void Plot::fill(Color color)
{

	for (int j = 1; j < adj_intersections.size(); j++) {
		for (int k = 2; k < adj_intersections.size(); k++) {
			Vector2 tr1 = { (float)adj_intersections[0]->coords.x, (float)adj_intersections[0]->coords.y };
			Vector2 tr2 = { (float)adj_intersections[j]->coords.x, (float)adj_intersections[j]->coords.y };
			Vector2 tr3 = { (float)adj_intersections[k]->coords.x, (float)adj_intersections[k]->coords.y };

			DrawTriangle(tr1, tr2, tr3, color);
			DrawTriangle(tr1, tr3, tr2, color);
		}
	}


}

void Plot::drawOcean()
{
	Color col = BLUE;

	for (int i = 0; i < actual_border.size() - 1; i++) {
		DrawTriangle({ (float)actualPlotCenter.x, (float)actualPlotCenter.y },
			{ (float)actual_border[i].x,(float)actual_border[i].y },
			{ (float)actual_border[i + 1].x, (float)actual_border[i + 1].y },
			col);
		DrawTriangle({ (float)actualPlotCenter.x, (float)actualPlotCenter.y },
			{ (float)actual_border[i+1].x,(float)actual_border[i+1].y },
			{ (float)actual_border[i].x, (float)actual_border[i].y },
			col);
	}
	DrawTriangle({ (float)actualPlotCenter.x, (float)actualPlotCenter.y },
		{ (float)actual_border.front().x,(float)actual_border.front().y},
		{ (float)actual_border.back().x, (float)actual_border.back().y},
		col);
	DrawTriangle({ (float)actualPlotCenter.x, (float)actualPlotCenter.y },
		{ (float)actual_border.back().x,(float)actual_border.back().y},
		{ (float)actual_border.front().x, (float)actual_border.front().y},
		col);
	
}

