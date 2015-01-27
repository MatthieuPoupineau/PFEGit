#include "stdafx.h"
#include "CollectionPointBus.h"


CollectionPointBus::CollectionPointBus(void)
{
}


CollectionPointBus::~CollectionPointBus(void)
{
}

int CollectionPointBus::getNodeId() const
{
    return nodeId;
}

void CollectionPointBus::setNodeId(int value)
{
    nodeId = value;
}

int CollectionPointBus::getWaitingPerson() const
{
    return waitingPerson;
}

void CollectionPointBus::setWaitingPerson(int value)
{
    waitingPerson = value;
}

int CollectionPointBus::getInstant() const
{
    return instant;
}

void CollectionPointBus::setInstant(int value)
{
    instant = value;
}

