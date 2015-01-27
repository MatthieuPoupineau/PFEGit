#pragma once
class CollectionPointCar
{
public:
	CollectionPointCar(void);
	~CollectionPointCar(void);

	int getNodeId() const;
    void setNodeId(int value);
	
	int getWaitingCar() const;
    void setWaitingCar(int value);

	int getInstant() const;
    void setInstant(int value);

protected:
private:
    int nodeId;
    int waitingCar;
    int instant;
};

