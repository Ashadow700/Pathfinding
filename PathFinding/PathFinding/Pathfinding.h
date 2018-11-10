#pragma once
#include "pch.h"
#include "Grid.h"
#include "Node.h"
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using std::vector;

class Pathfinding
{
public:
	Pathfinding(Grid& grid, RenderWindow& w);
	~Pathfinding();

	int execute();

private:
	Grid& grid;
	RenderWindow& w;
	vector< vector<Node> > nodes;
	vector<Node*> queue;
	Node* start;
	Node* goal;
	
	int begin();
	void dijkstra(Node* start, Node* goal);
	void generateNodes();
	void visitConnectedNodes(Node* currentNode);
	void pushToQueueIfNotGoal(Node* connectedNode);
	void updateGrid(Node* connectedNode);
	void updateGui(Node* currentNode);
	vector<Coordinate> generateConnections(int x, int y);
	int findNodeLowestTravelDistance();
	void createConnectionIfGridNotBlocked(vector<Coordinate>& connections, int x, int y);
	void drawPath(Node* goal);
	Node* getNode(int x, int y);
};

