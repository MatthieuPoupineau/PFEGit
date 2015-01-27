#include "stdafx.h"
#include "Pheromone.h"


Pheromone::Pheromone(void)
{
}
 
Pheromone::~Pheromone(void)
{
}
Path Pheromone::getPath() const
{
    return path;
}

void Pheromone::setPath(const Path &value)
{
    path = value;
}

int Pheromone::getIdDestination() const
{
	return idDestination;
}

void Pheromone::setIdDestination(int value)
{
	idDestination = value;
}

double Pheromone::getValue() const
{
    return value;
}

void Pheromone::setValue(double value)
{
    this->value = value;
}


