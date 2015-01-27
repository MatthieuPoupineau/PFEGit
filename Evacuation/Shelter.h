#pragma once
class Shelter
{
public:
	Shelter(void);
	~Shelter(void);
	int getNodeId() const;
    void setNodeId(int value);
	
	int getCarPlace() const;
    void setCarPlace(int value);

    int getTotalPlace() const;
    void setTotalPlace(int value);

protected:
private:
        int nodeId;
        int carPlace;
        int totalPlace;
};

