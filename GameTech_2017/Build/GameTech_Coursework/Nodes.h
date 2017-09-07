#pragma once
#include "..\nclgl\Vector3.h"

#include "Edge.h"

#include <vector>
#include <string>
#include <fstream>

using std::ifstream;
using std::string;
class Nodes
{
public:
	Nodes() {};
	Nodes(Vector3 v) { origin = v; };
	void setOrigin(Vector3 o) {
		origin = o;
		cost = 1;
	};
	void setNodeID(int id) { ID = id; };
	void setCost(float c) { cost = c; };
	float getCost() { return cost; };
	int getID() { return ID; };
	void setParent(int p) { Parent = p; }
	int getParent() { return Parent; }
	void setPass(bool p) { Pass = p; }
	bool getPass() { return Pass; }
	void addEdge(Edge e) { edges.push_back(e); }
	std::vector <Edge> getEdges() { return edges; };
	~Nodes() {
		delete node;
	};

	Vector3 getOrigin() { return origin; };
protected:
	std::vector <Edge> edges;
	int Parent = -1;
	bool Pass = true;
	int ID;
	float cost;
	Vector3 origin;
	Nodes* node;
};