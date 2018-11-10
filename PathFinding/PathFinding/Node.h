#pragma once
#include "pch.h"
#include "Coordinate.h"
#include <iostream>
#include <vector>

using std::vector;

class Node
{
public:
	Node();
	Node(int x, int y, vector<Coordinate> &connections, int status);
	~Node();

	int getX() const;
	int getY() const;
	bool canVisit();
	void setCanVisit(bool newStatus);
	int getStatus() const;
	void setPath(std::vector<Coordinate>& path);
	void addToPath(Coordinate& coordinate);
	vector<Coordinate> getPath() const;
	Coordinate getConnection(int index) const;
	Coordinate getCoordinate() const;
	int getMinDist() const;
	int getNumberOfConnectedNodes() const;

private:
	int x;
	int y;
	int minDist;
	int status;
	bool visitStatus;
	vector<Coordinate> connections;
	vector<Coordinate> path;
};