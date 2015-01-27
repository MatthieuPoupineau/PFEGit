#include "stdafx.h"
#include "Shelter.h"


Shelter::Shelter(void)
{
}


Shelter::~Shelter(void)
{
}

int Shelter::getNodeId() const
{
    return nodeId;
}

void Shelter::setNodeId(int value)
{
    nodeId = value;
}

int Shelter::getCarPlace() const
{
    return carPlace;
}

void Shelter::setCarPlace(int value)
{
    carPlace = value;
}

int Shelter::getTotalPlace() const
{
    return totalPlace;
}

void Shelter::setTotalPlace(int value)
{
    totalPlace = value;
}