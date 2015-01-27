#include "stdafx.h"
#include "City.h"


City::City(void)
{
}


City::~City(void)
{
}
int City::getDepotId() const
{
    return depotId;
}

void City::setDepotId(int value)
{
    depotId = value;
}

int City::getNbNodes() const
{
    return nbNodes;
}

void City::setNbNodes(int value)
{
    nbNodes = value;
}

vector<Edge> City::getVec_Edge() const
{
    return vec_Edge;
}

void City::setVec_Edge(const vector<Edge> &value)
{
    vec_Edge = value;
}

int City::getTimeHorizon() const
{
    return timeHorizon;
}

void City::setTimeHorizon(int value)
{
    timeHorizon = value;
}

int City::getNbBus() const
{
    return nbBus;
}

void City::setNbBus(int value)
{
    nbBus = value;
}

int City::getBusCapacity() const
{
    return busCapacity;
}

void City::setBusCapacity(int value)
{
    busCapacity = value;
}

int City::getNBPermittedShelter() const
{
    return nBPermittedShelter;
}

void City::setNBPermittedShelter(int value)
{
    nBPermittedShelter = value;
}

double City::getBusCarRatio() const
{
    return busCarRatio;
}

void City::setBusCarRatio(double value)
{
    busCarRatio = value;
}

int City::getMaxRisk() const
{
    return maxRisk;
}

void City::setMaxRisk(int value)
{
    maxRisk = value;
}

vector<CollectionPointBus> City::getVec_CollecBus() const
{
    return vec_CollecBus;
}

void City::setVec_CollecBus(const vector<CollectionPointBus> &value)
{
    vec_CollecBus = value;
}

vector<CollectionPointCar> City::getVec_CollecCar() const
{
    return vec_CollecCar;
}

void City::setVec_CollecCar(const vector<CollectionPointCar> &value)
{
    vec_CollecCar = value;
}

vector<Shelter> City::getVec_Shelter() const
{
    return vec_Shelter;
}

void City::setVec_Shelter(const vector<Shelter> &value)
{
    vec_Shelter = value;
}

vector<int> City::getVec_IdNode() const
{
    return vec_IdNode;
}

void City::setVec_IdNode(const vector<int> &value)
{
    vec_IdNode = value;
}

