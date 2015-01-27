#include "stdafx.h"
#include "Path.h"


Path::Path(void)
{
}


Path::~Path(void)
{
}

int Path::getInitialNode() const
{
    return initialNode;
}

void Path::setInitialNode(int value)
{
    initialNode = value;
}

int Path::getFinalNode() const
{
    return finalNode;
}

void Path::setFinalNode(int value)
{
    finalNode = value;
}
