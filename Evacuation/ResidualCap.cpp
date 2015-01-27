#include "stdafx.h"
#include "ResidualCap.h"


ResidualCap::ResidualCap(void)
{
}


ResidualCap::~ResidualCap(void)
{
}

Path ResidualCap::getPath() const
{
    return path;
}

void ResidualCap::setPath(Path value)
{
    path = value;
}

int ResidualCap::getCapacity() const
{
    return capacity;
}

void ResidualCap::setCapacity(int value)
{
    capacity = value;
}

int ResidualCap::getInstant() const
{
	return instant;
}

void ResidualCap::setInstant(int value)
{
    instant = value;
}