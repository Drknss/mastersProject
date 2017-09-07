#pragma once

#include "..\nclgl\GameTimer.h"

#include "Nodes.h"

using namespace std;
class AStar
{
public:
	AStar();
	void Initalize();
	vector<Nodes*> calcPath(Vector3 start, Vector3 end);
	float calcDistance(Vector3 start, Vector3 end);	
	void pathFinder(Vector3 start, Vector3 end);
	Nodes closestNode(Vector3 pos);
	~AStar();
	//Nodes* n = new Nodes[24];
	//Edge* e = new Edge[23];
private:	
	vector <Edge*> e;
	vector <Nodes*> n;
	vector <int> noPass;
	vector <Nodes*> closedListNodes;
	vector <Nodes*> openListNodes;
	float qg = 0;
	float qh = 0;
	float qf = 0;
	int nodeID = 0; //p
	int tempID = 0; //q
	bool update = false;
	bool pathFound = false;
};

