#pragma once
class CollectionPointBus
{
public:
	CollectionPointBus(void);
	~CollectionPointBus(void);

	int getNodeId() const;
    void setNodeId(int value);

    int getWaitingPerson() const;
    void setWaitingPerson(int value);

    int getInstant() const;
    void setInstant(int value);

protected:
private:
   int nodeId;
   int waitingPerson;
   int instant;
};

