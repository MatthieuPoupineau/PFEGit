#pragma once
#include <time.h>
#include <iostream>
#include <fstream>
#include "City.h"
#include "Pheromone.h"
#include "ResidualCap.h"
#include "Dijkstra.h"
#include "Bus.h"


class Heuristic1
{
public:
	Heuristic1(City city);
	~Heuristic1(void);

	void findSolutions();
	vector<int> CPBusSortByProximity(int initialNode);

private:
	City city;
	int NbColony;

	vector<int> vecShelterId;
	vector<int> vecCollectionPointId;
	vector<Pheromone> vecPheromoneCs; // pheromon matrix to Cs
	vector<Pheromone> vecPheromonePr; // pheromon matrix to Pr
	map<Path, int*> mapRCap; // residual capacity
	map<Path, int> mapTime; // Time for the key "path"
	map<Path, int> mapRisk; // Risk for the key "path"
	map<int, Bus> mapInfoBus; // bus information for the key "bus id"

	void initPheromoneMatrix();
	void initMapRCap();
	void initMapTimeAndMapRisk();
	void updateResidualCap(vector<int> vecPathToShelter, map<Path, int> mapDelay);
	void updateShelterCap(int idShelter, string transportType, int quantity);
	int updateTimeCurrentAnt(vector<int> vecPathToShelter, map<Path, int> mapDelay);
	void updatePheromoneMatrix(vector<int> vecPathToShelter, int timeAnt, int riskAnt);
	void displayPheromoneMatrix(vector<Pheromone> &vecPheromone);
	int riskCurrentAnt(vector<int> vecPathToShelter);
	int totalWaitingPerson();
	//vector<int> CPBusSortByProximity(int initialNode);
	vector<int> findLoop(vector<int> vecPathToShelter);
	vector<int> allowedNode(int previousNode, int initialNode, string transportType, int instant);
	int selectNextNode(int currentNode, vector<int> vecAllowedNode);
	void writeAntPath(int idAnt, vector<int> vecPathToShelter, int start, int timeCurrentAnt, int riskCurrentAnt);
};

