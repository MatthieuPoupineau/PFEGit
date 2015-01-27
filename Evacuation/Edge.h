#pragma once
#include "Path.h"
class Edge
{
public:
	Edge(void);
	~Edge(void);

	Path getPath() const;
    void setPath(Path path);

    int getCapacity() const;
    void setCapacity(int value);

    int getTime() const;
    void setTime(int value);

    int getRisk() const;
    void setRisk(int value);


private:
	Path path;
    int capacity;
    int time;
    int risk;
};

