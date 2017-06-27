/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  j2dias@uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Fall) 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    -
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <algorithm>
#include <limits>
#include "Exception.h"
#include "Disjoint_sets.h"

using namespace std;

//edge structure, has weight which connects nodes u and v
struct edge {
	double weight;
	int u;
	int v;
};

//functions I made outside of class
void heapsort(edge *edge,  int n);
void print(double **graph, int n);

class Weighted_graph {
	private:
		static const double INF;
		double** graph;//points to graph matrix
		int num_nodes;
		int num_edges;
		

		// Do not implement these functions!
		// By making these private and not implementing them, any attempt
		// to make copies or assignments will result in errors
		Weighted_graph( Weighted_graph const & );
		Weighted_graph &operator=( Weighted_graph );

		// your choice

	public:
		Weighted_graph( int = 10 );
		~Weighted_graph();

		int degree( int ) const;
		int edge_count() const;
		std::pair<double, int> minimum_spanning_tree() const;

bool insert_edge(int, int, double);
bool erase_edge(int, int);
void clear_edges();

// Friends

friend std::ostream &operator<<(std::ostream &, Weighted_graph const &);
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

Weighted_graph::Weighted_graph(int n) {
	//set graph as 2d matrix size nxn, poulate it with INF
	graph = new double*[n];
	for (int i = 0; i < n; i++) {
		graph[i] = new double[n];
		for (int j = 0; j < n; j++) {
			graph[i][j] = INF;
		}
	}
	num_nodes = n;
	num_edges = 0;
}

Weighted_graph::~Weighted_graph() {
	//de-allocate memory
	for (int c = 0; c < num_nodes; c++) {
		delete[] graph[c];
	}
	delete[] graph;
}

int Weighted_graph::degree(int u) const {
	//check to make sure element are in bounds
	if (u >= num_nodes || u<0 ) throw illegal_argument();
	//find degree of u;
	int ctr = 0;
	for (int c = 0; c < num_nodes; c++) {
		if (graph[u][c] != INF) ctr++;
	}
	return ctr;
}

int Weighted_graph::edge_count() const {
	return num_edges;
}

bool Weighted_graph::erase_edge(int i, int j) {
	//make sure nodes are in bounds
	if (i >= num_nodes || j >= num_nodes||i<0||j<0) throw illegal_argument();

	if (i == j) return true;
	if (graph[i][j] == INF) return false;

	//set both upper and lower triangular parts to INF
	graph[i][j] = INF;
	graph[j][i] = INF;
	num_edges--;
	//print(graph, num_nodes);
	return true;
}

void Weighted_graph::clear_edges() {
	//Set whole matrix to INF
	for (int i = 0; i < num_nodes; i++) {
		for (int j = 0; j < num_nodes; j++) {
			graph[i][j] = INF;
		}
	}
	num_edges = 0;
	//print(graph, num_nodes);
	return;
}

bool Weighted_graph::insert_edge(int i, int j, double d) {
	//check bounds
	if (i >= num_nodes || j >= num_nodes || i<0 || j<0 || d<0) throw illegal_argument();
	if (i == j) return false;
	if (graph[i][j]==INF) num_edges++;
	graph[i][j] = d;
	graph[j][i] = d;
	//print(graph, num_nodes);
	return true;
}

std::pair<double, int> Weighted_graph::minimum_spanning_tree() const {
	//no edges, no mst
	if (num_edges == 0) return std::pair<double, int>(0.0, 0);
	
	//variables to be returned
	double weight = 0;
	int visited = 0;

	//edge data structure to keep track of connecting nodes
	edge* edges = new edge[num_edges];
	//keep track of pos in array
	int ctr = 0;


	//Place edge weights in array, along with connecting nodes: O(n)
	for (int i = 0; i < num_nodes - 1; i++) {
		for (int j = i + 1; j < num_nodes; j++) {
			if (graph[i][j] != INF) {
				edges[ctr].weight = graph[i][j];
				edges[ctr].u = i;
				edges[ctr].v = j;
				ctr++;
			}
		}
	}

	//sort graph by edge weight, inceasing order
	heapsort(edges, num_edges);

	//use kruskal's algorithm to find MST by using disjoint set
	Disjoint_set *set = new Disjoint_set(num_nodes);
	
	//variable to keep track of previous number of disjoint sets
	int old_numsets;
	//go through each edge until mst is complete, or there are no more edges
	for (int c = 0; c < num_edges; c++) {
		visited++;
		old_numsets = set->num_sets();
		set->union_sets(edges[c].u, edges[c].v);
		if (set->num_sets() != old_numsets) {//means sets were unioned, which means the edge didn;t create a cycle and was added to mst
			weight += edges[c].weight;
		}
		
		if (set->num_sets() == 1) {//mst is complete
			break;
		}
	}

	//de-allocate memory
	delete set;
	delete[] edges;
	return std::pair<double, int>( weight, visited );
}

//print adjacency matrix
void print(double **graph, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << graph[i][j]<<"  ";
		}
		cout << "\n";
	}
}

//turn array into max heap
void heapify(edge *edges, int i, int n) {
	int largest;
	int l = 2 * i;
	int r = 2 * i + 1;
	
	//find largest
	if (l<n && edges[l].weight>edges[i].weight) {
		largest = l;
	}
	else {
		largest = i;
	}
	if (r < n && edges[r].weight > edges[largest].weight) {
		largest = r;
	}
	if (largest != i) {
		swap(edges[i], edges[largest]);
		heapify(edges,  largest, n);
	}
}

void heapsort(edge *edges, int n) {
	//build max heap
	for (int i = n / 2; i >= 0; i--) {
		heapify(edges, i, n);
	}

	//build sorted array with max heap
	for (int i = n-1; i >1; i--) {
		swap(edges[0], edges[i]);
		heapify(edges, 0, i-1);
	}
}

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	

	return out;
}


#endif