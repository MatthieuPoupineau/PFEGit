#pragma once

class Path
{
public:
	Path(void);
	~Path(void);

	int getInitialNode() const;
    void setInitialNode(int value);
	
	int getFinalNode() const;
    void setFinalNode(int value);
	
	// Redefine the operator "<" for using map<Path, int>
	friend bool operator<(Path p1, Path p2)
    {
        	if(p1.initialNode != p2.initialNode)
		return p1.initialNode < p2.initialNode;
	else 
		return p1.finalNode < p2.finalNode;
    }
/*inline bool operator<(const Path &p1) 
{
	if(this->initialNode != p1.initialNode)
		return this->initialNode < p1.initialNode;
	else 
		return this->finalNode < p1.finalNode;

}

	inline bool operator>(const Path &p1) 
{
	if(this->initialNode != p1.initialNode)
		return this->initialNode > p1.initialNode;
	else 
		return this->finalNode > p1.finalNode;

}
inline bool operator==(const Path &p1) 
{
		return this->initialNode == p1.initialNode && this->finalNode == p1.finalNode;
}

inline bool operator!=(Path &p1) 
{
		return this->initialNode != p1.initialNode || this->finalNode != p1.finalNode;
}
*/
private:

	int initialNode;
    int finalNode;
};

