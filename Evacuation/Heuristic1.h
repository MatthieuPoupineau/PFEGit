#pragma once
#include <time.h>
#include <iostream>
#include <fstream>
#include "City.h"
#include "Pheromone.h"
#include "ResidualCap.h"
#include "Dijkstra.h"

using namespace std;

class Heuristic1
{
public:
	Heuristic1(City city);
	~Heuristic1(void);

	void findSolutions();

private:
	City city;
	int NbColony;

	vector<int> vecShelterId;
	vector<int> vecCollectionPointId;
	vector<Pheromone> vecPheromoneCs; // pheromon matrix to Cs
	vector<Pheromone> vecPheromonePr; // pheromon matrix to Pr
	map<Path, int*> mapRCap; // residual capacity
	map<Path, int> mapTime;
	map<Path, int> mapRisk;

	void initPheromoneMatrix();
	void initMapRCap();
	void initMapTimeAndMapRisk();
	void updateResidualCap(Path path, int instant, int time);
	void updatePheromoneMatrix(vector<int> vecPathToShelter, int timeAnt, int riskAnt);
	void displayPheromoneMatrix(vector<Pheromone> &vecPheromone);
	vector<int> findLoop(vector<int> vecPathToShelter, int* timeCurrentAnt, int *riskCurrentAnt);
	vector<int> allowedNode(int previousNode, int initialNode, string transportType, int instant);
	int selectNextNode(int currentNode, vector<int> vecAllowedNode);
	void writeAntPath(int idAnt, vector<int> vecPathToShelter, int timeCurrentAnt, int riskCurrentAnt);
};

