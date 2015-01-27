#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "City.h"

using namespace std;

class Parser
{
public:
	Parser(void);
	~Parser(void);
	static City* parse(string fileName);

protected:
private:
    static int getDepotId(string d_depot);
    static vector<int> getNodes(string d_nodes);
    static vector<Edge> getEdge(string d_edges);
	static int getTimeHorizon(string d_time_horizon);
    static int getNbBus(string d_number_of_buses);
    static int getBusCapacity(string d_bus_capacity);
    static double getBusCarRatio(string d_bus_to_car_ratio);
    static vector<CollectionPointBus> getCollectBus(string d_bus_demand_list);
	static vector<CollectionPointCar> getCollectCar(string d_car_demand_list);
    static vector<Shelter> getShelter(string d_shelter_capacity_list);
    static void displayCity(City city);
	
};