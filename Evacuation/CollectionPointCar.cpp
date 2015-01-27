#include "stdafx.h"
#include "CollectionPointCar.h"


CollectionPointCar::CollectionPointCar(void)
{
}


CollectionPointCar::~CollectionPointCar(void)
{
}

int CollectionPointCar::getNodeId() const
{
    return nodeId;
}

void CollectionPointCar::setNodeId(int value)
{
    nodeId = value;
}

int CollectionPointCar::getWaitingCar() const
{
    return waitingCar;
}

void CollectionPointCar::setWaitingCar(int value)
{
    waitingCar = value;
}

int CollectionPointCar::getInstant() const
{
    return instant;
}

void CollectionPointCar::setInstant(int value)
{
    instant = value;
}
