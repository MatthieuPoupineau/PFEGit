#pragma once

#include "Edge.h";

class Pheromone
{
public:
	Pheromone(void);
	~Pheromone(void);
	Path getPath() const;
    void setPath(const Path &value);
	
	int getIdDestination() const;
    void setIdDestination(int value);

    double getValue() const;
    void setValue(double value);

private:
    Path path;
	int idDestination;
    double value;
};

