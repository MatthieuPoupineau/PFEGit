#pragma once
#include <vector>
#include <map>
#include "Shelter.h"
#include "Edge.h"
#include "CollectionPointBus.h"
#include "CollectionPointCar.h"
using namespace std;
class City
{
public:
	City(void);
	~City(void);

	int getDepotId() const;
    void setDepotId(int value);

    int getNbNodes() const;
    void setNbNodes(int value);

    vector<Edge> getVec_Edge() const;
    void setVec_Edge(const vector<Edge> &value);

	int getTimeHorizon() const;
    void setTimeHorizon(int value);

    int getNbBus() const;
    void setNbBus(int value);

    int getBusCapacity() const;
    void setBusCapacity(int value);

    int getNBPermittedShelter() const;
    void setNBPermittedShelter(int value);

    double getBusCarRatio() const;
    void setBusCarRatio(double value);

    int getMaxRisk() const;
    void setMaxRisk(int value);

    vector<CollectionPointBus> getVec_CollecBus() const;
    void setVec_CollecBus(const vector<CollectionPointBus> &value);

    vector<CollectionPointCar> getVec_CollecCar() const;
    void setVec_CollecCar(const vector<CollectionPointCar> &value);

    vector<Shelter> getVec_Shelter() const;
    void setVec_Shelter(const vector<Shelter> &value);

    vector<int> getVec_IdNode() const;
    void setVec_IdNode(const vector<int> &value);

    int getNbShelter() const;
    void setNbShelter(int value);

    int getNbCollectionPoint() const;
    void setNbCollectionPoint(int value);

protected:
private:
    int depotId;
    int nbNodes; // number of nodes in the city
    vector<int> vec_IdNode;
    vector<Edge> vec_Edge;
    int timeHorizon;
    int nbBus;
	int nbShelter;
	int nbCollectionPoint;
    int busCapacity;
    int nBPermittedShelter;
    double busCarRatio;
    int maxRisk;
    vector<CollectionPointBus> vec_CollecBus;
    vector<CollectionPointCar> vec_CollecCar;
    vector<Shelter> vec_Shelter;
};


