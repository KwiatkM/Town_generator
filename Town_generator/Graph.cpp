#include "Graph.h"
#include "point_gen.h"


//---------------------------------------------------------------
// 
//                        KONSTRUKTORY
// 
//---------------------------------------------------------------
Graph::Graph() {

}

Graph::Graph(int n_points, point_t* points, elem_t** adj_list) {
	this->n_points = n_points;
	this->points = points;
	this->adj_list = adj_list;
}

Graph::Graph(int n_points, point_t* points) {
	this->n_points = n_points;
	this->points = points;
	adj_list = new elem_t* [n_points];
	for (int i = 0; i < n_points; i++) {
		adj_list[i] = NULL;
	}
}

Graph::Graph(int n_points) {
	this->n_points = n_points;
	points = new point_t[n_points];
	adj_list = new elem_t* [n_points];
	for (int i = 0; i < n_points; i++) {
		adj_list[i] = NULL;
	}
}

//---------------------------------------------------------------
// 
//                           SETTERY
// 
//---------------------------------------------------------------

void Graph::SetN_points(uint16_t n_points) {
	this->n_points = n_points;
}

//---------------------------------------------------------------
// 
//                           GETERY
// 
//---------------------------------------------------------------

point_t Graph::GetPoint(int point_nr) {
	if ((point_nr > n_points - 1) || (point_nr < 0)) {
		return {1,1}; // do poprawy
	}
	else {
		return points[point_nr];
	}
}

elem_t** Graph::getAdj_list(){
	return adj_list;
}

//---------------------------------------------------------------
// 
//                           METODY
// 
//---------------------------------------------------------------

void Graph::generate_points(int max_x, int max_y) {
	uniform_point_gen(points, n_points, max_x, max_y );
}

void Graph::add_conection(int point1, int point2) {
	elem_t* new_e = new elem_t;
	new_e->value = point2;
	elem_t* head = adj_list[point1];
	
	if (head == NULL) {
		adj_list[point1] = new_e;
	}
	else {
		while (head->next != NULL) {
			head = head->next;
		}
		head->next = new_e;
	}
}








void Graph::free() {
	delete[] points;
	for (int i = 0; i < n_points; i++) {
		elem_t* head = adj_list[i];
		elem_t* tmp;
		while (head != NULL) {
			tmp = head;
			head = head->next;
			delete tmp;
		}
	}

	delete[] adj_list;
}