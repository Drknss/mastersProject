#pragma once
#include "..\nclgl\Vector3.h"

#include <string>

using namespace std;

class Edge
{
public:
	Edge() {};
	Edge(Vector3 s, Vector3 e) { start = s; end = e; }
	~Edge() {};
	Vector3 getStart() { return start; };
	Vector3 getEnd() { return end; };
	string getID() { return ID; };
	void setID(string id) { ID = id; }
	inline bool	operator==(Edge &A)const { if ((this->start == A.getStart() && this->end == A.getEnd()) || (this->start == A.getEnd() && this->end == A.getStart())) { return true; } return false; };
protected:
	string ID;
	Vector3 start;
	Vector3 end;
};