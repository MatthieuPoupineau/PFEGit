#pragma once
class Bus
{
public:
	Bus(void);
	~Bus(void);

    int getCurrentNode() const;
    void setCurrentNode(int value);

    int getNbPersonInBus() const;
    void setNbPersonInBus(int value);

    int getInstant() const;
    void setInstant(int value);

private:
    int currentNode;
    int nbPersonInBus;
    int instant;
};

