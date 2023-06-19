#include "TGenCanvas.h"

TGenCanvas::TGenCanvas(){
}

TGenCanvas::TGenCanvas(TownMap* TMap_) {
	TMap = TMap_;
}

void TGenCanvas::drawDefault()
{
	//drawPlotsDebug();
	drawInfrostructure();
	//drawRoads();
}

void TGenCanvas::drawEdit()
{
	drawPlotsDebug();
	//drawRoads();
	drawIntersectionsAsPoints();
	listenEdit();
}

void TGenCanvas::drawManual()
{
	drawPlotCenters();
	listenManual();
}

void TGenCanvas::drawDebug()
{
	drawPlotCenters();
	drawIntersectionsAsPoints();
	drawConnectionDebug();
}

void TGenCanvas::drawConnectionDebug()
{
	for (Triangle& t : TMap->triangles) {
		DrawLine(t.p1->x, t.p1->y, t.p2->x, t.p2->y, BLACK);
		DrawLine(t.p2->x, t.p2->y,t.p3->x, t.p3->y,BLACK);
		DrawLine(t.p1->x, t.p1->y,t.p3->x, t.p3->y,BLACK);
	}
	for (Intersection &i : TMap->intersections) {
		for (Intersection* ic : i.connected_intersections) {
			DrawLine(i.coords.x, i.coords.y, ic->coords.x, ic->coords.y, BLUE);
		}
	}
}

void TGenCanvas::drawPlotCenters()
{
	for (point_t &p : TMap->plot_centers) {
		DrawCircle(p.x, p.y, 4, BLACK);
	}
}

void TGenCanvas::drawIntersectionsAsPoints()
{
	for (Intersection& i : TMap->intersections) {
		DrawCircle(i.coords.x, i.coords.y, 4, RED);
	}
}

void TGenCanvas::drawRiver()
{
	if (TMap->riverEnd != nullptr) {

	
	Color riverColor = BLUE;
	Intersection* i = TMap->riverEnd;

	int riverWidth = (TMap->canvas_height * TMap->canvas_width)/ 52000.0; //byæ mo¿e do zmiany

	DrawCircle(i->coords.x, i->coords.y, riverWidth / 2, riverColor);
	while (i->previousIntersection != i) {
		DrawLineEx({ (float)i->coords.x, (float)i->coords.y },
			{ (float)i->previousIntersection->coords.x, (float)(i->previousIntersection->coords.y) },
			riverWidth,
			riverColor);
		DrawCircle(i->previousIntersection->coords.x, i->previousIntersection->coords.y, riverWidth / 2, riverColor);
		i = i->previousIntersection;
	}
	}
}

void TGenCanvas::drawOutsideConnections()
{
	int outConWidth = (TMap->canvas_height * TMap->canvas_width) / 34600.0;
	Color outConColor = { 0xdd, 0xb8, 0x8b, 0xff };
	for (std::vector<Intersection*> Vi : TMap->outsideConnections) {

		DrawCircle(Vi[0]->coords.x, Vi[0]->coords.y, outConWidth / 2, outConColor);
		for (int i = 1; i < Vi.size(); i++) {
			DrawCircle(Vi[i]->coords.x, Vi[i]->coords.y, outConWidth / 2, outConColor);
			DrawLineEx({ (float)Vi[i]->coords.x, (float)Vi[i]->coords.y },
				{ (float)Vi[i - 1]->coords.x, (float)Vi[i - 1]->coords.y },
				outConWidth,
				outConColor);
		}
		
	}
}

void TGenCanvas::drawWalls()
{
}

void TGenCanvas::drawInfrostructure()
{
	for (Plot& p : TMap->plots) {
		p.draw();
	}
	
	drawOutsideConnections();
	drawRiver();
}

void TGenCanvas::drawPlotsDebug()
{
	Color col;
	
	for (Plot &p : TMap->plots) {
		col = { 0xE3, 0xB2, 0x3C, 0xff };

		if (p.type == P_RESIDENTIAL) {
			for (int j = 1; j < p.adj_intersections.size(); j++) {
				for (int k = 2; k < p.adj_intersections.size(); k++) {
					Vector2 tr1 = { (float)p.actual_border[0].x, (float)p.actual_border[0].y };
					Vector2 tr2 = { (float)p.actual_border[j].x, (float)p.actual_border[j].y };
					Vector2 tr3 = { (float)p.actual_border[k].x, (float)p.actual_border[k].y };

					DrawTriangle(tr1, tr2, tr3, col);
					DrawTriangle(tr1, tr3, tr2, col);
				}
			}
		}
	}
}

/*
void TGenCanvas::drawRoads() //do zmiany
{
	Color col;
	for (Intersection& i : TMap->intersections) {
		if (i.type != ROAD) {
			col = i.getColor();
			intersection_type currentType = i.type;
			for (Intersection* ic : i.connected_intersections) {
				if (ic->type == currentType) {
					DrawLineEx({ (float)i.coords.x,(float)i.coords.y }, { (float)ic->coords.x,(float)ic->coords.y },
						3, col);
				}
			}
		}
	}
}*/

void TGenCanvas::listenManual()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		if (GetMouseX() < TMap->canvas_width)
			TMap->plot_centers.push_back({ GetMouseX() ,GetMouseY() });
	
}


void TGenCanvas::listenEdit()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		for (Intersection& i : TMap->intersections) {
			if (dist2(i.coords, { GetMouseX() ,GetMouseY() }) < 36) {
				E_tmp = &i;
				E_pressed = true;
				break;
			}
		}
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && E_pressed) {
		E_tmp->coords = { GetMouseX() ,GetMouseY() };
		for (Plot* p : E_tmp->adj_plots) {
			p->calculateActualSortedBorders();
		}
	}
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		
		E_pressed = false;
		E_tmp = nullptr;

	}
}




