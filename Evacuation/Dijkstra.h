#pragma once
#include <vector>
#include "Edge.h"

using namespace std;
class Dijkstra
{
public:
	Dijkstra(void);
	~Dijkstra(void);

	int* calculateAll(int initialNode, vector<Edge> vecEdges, int nbNode);
	int calculate(int initialNode, int finalNode, vector<Edge> vecEdges, int nbNode);
	int minDistance(int dist[], bool sptSet[], int nbNode);
	int ** vecEdgesToGraph(vector<Edge> vecEdges, int nbNode);
};

