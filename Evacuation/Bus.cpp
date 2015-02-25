#include "stdafx.h"
#include "Bus.h"


Bus::Bus(void)
{
}


Bus::~Bus(void)
{
}

int Bus::getCurrentNode() const
{
    return currentNode;
}

void Bus::setCurrentNode(int value)
{
    currentNode = value;
}
int Bus::getNbPersonInBus() const
{
    return nbPersonInBus;
}

void Bus::setNbPersonInBus(int value)
{
    nbPersonInBus = value;
}
int Bus::getInstant() const
{
    return instant;
}

void Bus::setInstant(int value)
{
    instant = value;
}


