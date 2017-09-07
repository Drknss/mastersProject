#include "AStar.h"

AStar::AStar()
{
	Initalize();
}

void AStar::Initalize() {

#pragma region Nodes
	n.push_back(new Nodes(Vector3(-12.5, 1.1, 13)));
	n.push_back(new Nodes(Vector3(-12.5, 1.1, 7.5)));
	n.push_back(new Nodes(Vector3(-12.5, 1.1, 2.5)));
	n.push_back(new Nodes(Vector3(-12.5, 1.1, -5.5)));
	n.push_back(new Nodes(Vector3(-12.5, 1.1, -10)));
	n.push_back(new Nodes(Vector3(-12.5, 1.1, -12.5)));
	n.push_back(new Nodes(Vector3(-6.5, 1.1, 7.5)));
	n.push_back(new Nodes(Vector3(-6.5, 1.1, 2.5)));
	n.push_back(new Nodes(Vector3(-6.5, 1.1, -2.5)));
	n.push_back(new Nodes(Vector3(-6.5, 1.1, -7)));
	n.push_back(new Nodes(Vector3(1.5, 1.1, 13)));
	n.push_back(new Nodes(Vector3(1.5, 1.1, 2.5)));
	n.push_back(new Nodes(Vector3(1.5, 1.1, -4)));
	n.push_back(new Nodes(Vector3(1.5, 1.1, -7.5)));
	n.push_back(new Nodes(Vector3(1.5, 1.1, -12.5)));
	n.push_back(new Nodes(Vector3(7, 1.1, 7)));
	n.push_back(new Nodes(Vector3(7, 1.1, 3)));
	n.push_back(new Nodes(Vector3(7, 1.1, 1)));
	n.push_back(new Nodes(Vector3(13, 1.1, 12.5)));
	n.push_back(new Nodes(Vector3(13, 1.1, 10)));
	n.push_back(new Nodes(Vector3(13, 1.1, 3)));
	n.push_back(new Nodes(Vector3(13, 1.1, -4)));
	n.push_back(new Nodes(Vector3(13, 1.1, -9.5)));
	n.push_back(new Nodes(Vector3(13, 1.1, -12.5)));

	for (int i = 0; i < 24; i++) {
		n[i]->setNodeID(i);
	}

#pragma endregion Nodes

#pragma region Edges
	e.push_back(new Edge(n[0]->getOrigin(), n[1]->getOrigin()));
	e.push_back(new Edge(n[2]->getOrigin(), n[3]->getOrigin()));
	e.push_back(new Edge(n[4]->getOrigin(), n[5]->getOrigin()));
	e.push_back(new Edge(n[0]->getOrigin(), n[10]->getOrigin()));
	e.push_back(new Edge(n[6]->getOrigin(), n[7]->getOrigin()));
	e.push_back(new Edge(n[2]->getOrigin(), n[7]->getOrigin()));
	e.push_back(new Edge(n[8]->getOrigin(), n[9]->getOrigin()));
	e.push_back(new Edge(n[5]->getOrigin(), n[14]->getOrigin()));
	e.push_back(new Edge(n[9]->getOrigin(), n[13]->getOrigin()));
	e.push_back(new Edge(n[7]->getOrigin(), n[11]->getOrigin()));
	e.push_back(new Edge(n[13]->getOrigin(), n[14]->getOrigin()));
	e.push_back(new Edge(n[12]->getOrigin(), n[13]->getOrigin()));
	e.push_back(new Edge(n[11]->getOrigin(), n[12]->getOrigin()));
	e.push_back(new Edge(n[10]->getOrigin(), n[11]->getOrigin()));
	e.push_back(new Edge(n[10]->getOrigin(), n[18]->getOrigin()));
	e.push_back(new Edge(n[15]->getOrigin(), n[16]->getOrigin()));
	e.push_back(new Edge(n[16]->getOrigin(), n[17]->getOrigin()));
	e.push_back(new Edge(n[16]->getOrigin(), n[20]->getOrigin()));
	e.push_back(new Edge(n[20]->getOrigin(), n[21]->getOrigin()));
	e.push_back(new Edge(n[21]->getOrigin(), n[12]->getOrigin()));
	e.push_back(new Edge(n[18]->getOrigin(), n[19]->getOrigin()));
	e.push_back(new Edge(n[14]->getOrigin(), n[23]->getOrigin()));
	e.push_back(new Edge(n[22]->getOrigin(), n[23]->getOrigin()));
	e.push_back(new Edge(n[00]->getOrigin(), n[21]->getOrigin()));

	for (int no = 0; no < 24; no++) {
		for (int ed = 0; ed < 23; ed++) {
			if (n[no]->getOrigin() == e[ed]->getStart() || n[no]->getOrigin() == e[ed]->getEnd()) {
				n[no]->addEdge(*e[ed]);
			}
		}
	}

#pragma endregion Edges
}


vector<Nodes*> AStar::calcPath(Vector3 start, Vector3 end)
{
}

float AStar::calcDistance(Vector3 start, Vector3 end)
{
	float dist = 0;
	pathFinder(start, end);
	if (closedListNodes.size() > 2) {
		for (int i = 0; i != closedListNodes.size() - 1; ++i) {
			dist = std::abs (dist + Vector3::Distance(closedListNodes[i]->getOrigin(), closedListNodes[i + 1]->getOrigin()));
		}
	}
	else {
		dist = Vector3::Distance(closedListNodes[0]->getOrigin(), closedListNodes[1]->getOrigin());
	}
	return dist;
}

void AStar::pathFinder(Vector3 startPos, Vector3 endPos) {
	GameTimer* t = new GameTimer();
	//A*
	int start = closestNode(startPos).getID();
	int end = closestNode(endPos).getID();
	closedListNodes.clear();
	openListNodes.clear();
	float qg = 0;
	float qh = 0;
	float qf = 0;
	int nodeID = 0; //p
	int tempID = 0; //q
	bool update = false;
	bool pathFound = false;
	openListNodes.push_back(n[start]);
	while (!openListNodes.empty()) {
		float msec = t->GetTimedMS();
		int count = 0;
		float fvmin = 1000;
		for each(Nodes* nd in openListNodes) {
			float fv = Vector3().Distance(nd->getOrigin(), n[end]->getOrigin()) + nd->getCost();
			if (fv < fvmin) {
				nodeID = nd->getID();
				fvmin = fv;
				openListNodes.erase(openListNodes.begin() + count);
				closedListNodes.push_back(n[nodeID]);
				break;
			}
			count++;
		}
		if (nodeID == end) {
			closedListNodes.push_back(n[nodeID]);
			pathFound = true;
			break;
		}
		if (msec / 10000 >= 1) {
			cout << "Unable to find a path in a reasonable time!!" << endl;
			pathFound = false;
			break;
		}
		for each(Edge ed in n[nodeID]->getEdges()) {
			if (ed.getStart() != n[nodeID]->getOrigin()) {
				for (int i = 0; i < n.size(); i++) {
					if (ed.getStart() == n[i]->getOrigin()) {
						tempID = n[i]->getID();
						qg = 1;
						qh = Vector3().Distance(n[tempID]->getOrigin(), n[end]->getOrigin());
						qf = qh + qg;
					}
				}
			}
			else if (ed.getEnd() != n[nodeID]->getOrigin()) {
				for (int i = 0; i < n.size(); i++) {
					if (ed.getEnd() == n[i]->getOrigin()) {
						tempID = n[i]->getID();
						qg = 1;
						qh = Vector3().Distance(n[tempID]->getOrigin(), n[end]->getOrigin());
						qf = qh + qg;
					}
				}
			}
			if (!n[tempID]->getPass()) {
				continue;
			}
			for each (Nodes* no in closedListNodes) {
				if (tempID == no->getID()) {
					if (qg < no->getCost()) {
						no->setParent(tempID);
					}
					update = true;
				}
			}
			for each (Nodes* no in openListNodes) {
				if (tempID == no->getID()) {
					if (qg < no->getCost()) {
						no->setParent(tempID);
					}
					update = true;
				}
			}
			if (!update) {
				n[tempID]->setParent(nodeID);
				openListNodes.push_back(n[tempID]);
			}
			update = false;
		}
	}
}

Nodes AStar::closestNode(Vector3 pos) {
	float dis = 0;
	float minDis = 100000;
	int count = 0;
	int result = 0;
	for each(Nodes* ni in n){
		float dis = Vector3::Distance(pos, n[count]->getOrigin());
		if (dis < minDis) {
			minDis = dis;
			result = count;
		}
		count++;

	}
	return *n[result];
}


AStar::~AStar()
{
}
