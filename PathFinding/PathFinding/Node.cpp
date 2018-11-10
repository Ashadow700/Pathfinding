#include "pch.h"
#include "Node.h"
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>


Node::Node(int x, int y, std::vector<Coordinate> &connections, int status) :
	x(x),
	y(y),
	connections(connections),
	status(status) {
	minDist = std::numeric_limits<int>::max();
	visitStatus = true;
}

Node::Node() {};

Node::~Node()
{
}

int Node::getX() const {
	return x;
}
int Node::getY() const {
	return y;
}

bool Node::canVisit() {
	return visitStatus;
}

void Node::setCanVisit(bool newStatus) {
	BOOST_LOG_TRIVIAL(trace) << "Setting can visit status to: " << newStatus;
	visitStatus = newStatus;
}

int Node::getStatus() const {
	return status;
}

int Node::getNumberOfConnectedNodes() const {
	return connections.size();
}

void Node::setPath(std::vector<Coordinate>& path) {
	Node::path = path;
	minDist = path.size();
	BOOST_LOG_TRIVIAL(trace) << "Setting new path";
}

void Node::addToPath(Coordinate& coordinate) {
	path.push_back(coordinate);
	minDist = path.size();
	BOOST_LOG_TRIVIAL(trace) << "Adding coordinate  [" << coordinate.getX() << "," << coordinate.getY() << "] to path";
}

std::vector<Coordinate> Node::getPath() const {
	return path;
}

Coordinate Node::getConnection(int index) const {
	return connections[index];
}

Coordinate Node::getCoordinate() const {
	return Coordinate{ x, y };
}

int Node::getMinDist() const {
	return minDist;
}