#include "stdafx.h"
#include "Edge.h"


Edge::Edge(void)
{
}


Edge::~Edge(void)
{
}

Path Edge::getPath() const
{
    return path;
}

void Edge::setPath(Path value)
{
    path = value;
}

int Edge::getCapacity() const
{
    return capacity;
}

void Edge::setCapacity(int value)
{
    capacity = value;
}

int Edge::getTime() const
{
    return time;
}

void Edge::setTime(int value)
{
    time = value;
}

int Edge::getRisk() const
{
    return risk;
}

void Edge::setRisk(int value)
{
    risk = value;
}
