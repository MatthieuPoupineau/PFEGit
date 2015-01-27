#pragma once
#include "Path.h"
class ResidualCap
{
public:
	ResidualCap(void);
	~ResidualCap(void);

	Path getPath() const;
    void setPath(Path path);

    int getCapacity() const;
    void setCapacity(int value);

    int getInstant() const;
    void setInstant(int value);

private:
	Path path;
    int capacity;
    int instant;
};

