#include "pch.h"
#include "GuiHandler.h";
#include <thread>
#include "TileStatus.h"
#include "Pathfinding.h"


Pathfinding::Pathfinding(Grid& grid, RenderWindow& w) :
	grid(grid),
	w(w) {

	nodes.resize(grid.getMaxX());
	for (int x = 0; x < grid.getMaxX(); ++x) {
		nodes[x].resize(grid.getMaxY());
	}
}

Pathfinding::~Pathfinding()
{
}

int Pathfinding::execute() {
	std::thread pathThread(&Pathfinding::begin, this);
	pathThread.detach();
	return 0;
}

int Pathfinding::begin() {
	BOOST_LOG_TRIVIAL(trace) << "Running Pathfinding::begin()";
	GuiHandler::setInputAllowed(false);
	generateNodes();
	start = getNode(grid.getStart().getX(), grid.getStart().getY());
	goal = getNode(grid.getGoal().getX(), grid.getGoal().getY());

	std::vector<Coordinate> travelDistance{};
	start->setPath(travelDistance);
	queue.push_back(start);

	dijkstra(start, goal);
	grid.setNeedsCleaning(true);
	GuiHandler::setInputAllowed(true);

	if (goal->getMinDist() == std::numeric_limits<int>::max()) {
		BOOST_LOG_TRIVIAL(info) << "No path to goal found";
		return (-1);
	} else {
		drawPath(goal);
		BOOST_LOG_TRIVIAL(info) << "Shortest path found: " << goal->getMinDist() << endl;
		return goal->getMinDist();
	}
}

void Pathfinding::dijkstra(Node* start, Node* goal) {
	BOOST_LOG_TRIVIAL(debug) << "Running Pathfinding::dijkstra(), start = [" << start->getX() << ", " << start->getY() << "], goal = [" << goal->getX() << ", " << goal->getY() << "]";

	while (queue.size() != 0) {
		
		int indexLowestNode = findNodeLowestTravelDistance();
		Node* currentNode = queue[findNodeLowestTravelDistance()];
		BOOST_LOG_TRIVIAL(debug) << "Selecting current node at [" << currentNode->getX() << "," << currentNode->getY() << "]";
		
		Coordinate currentCoordinate = currentNode->getCoordinate();
		currentNode->addToPath(currentCoordinate);
		visitConnectedNodes(currentNode);

		currentNode->setCanVisit(false);
		queue.erase(queue.begin()+indexLowestNode);
		BOOST_LOG_TRIVIAL(debug) << "Removing current node from queue";
		
		updateGui(currentNode);
	}
}


int Pathfinding::findNodeLowestTravelDistance() {
	BOOST_LOG_TRIVIAL(trace) << "Running Pathfinding::findNodeLowestTravelDistance()";
	int indexLowest = 0;
	Node* lowestNode = queue[0];

	for (int index = 0; index < queue.size(); ++index) {
		Node* foundNode = queue[index];

		BOOST_LOG_TRIVIAL(debug) << "Distance to node found = " << foundNode->getMinDist();
		if (foundNode->getMinDist() < lowestNode->getMinDist()) {
			BOOST_LOG_TRIVIAL(debug) << "Distance [" << foundNode->getMinDist() << "] to found node is lower then previous lowest of [" << lowestNode->getMinDist() << "]";
			lowestNode = foundNode;
			indexLowest = index;
		}
	}
	return indexLowest;
}

void Pathfinding::visitConnectedNodes(Node* currentNode) {
	BOOST_LOG_TRIVIAL(trace) << "Running Pathfinding::visitConnectedNodes()";

	vector<Coordinate> currentPath = currentNode->getPath();
	for (int index = 0; index < currentNode->getNumberOfConnectedNodes(); ++index) {
		Coordinate con = currentNode->getConnection(index);
		Node* connectedNode = getNode(con.getX(), con.getY());
		BOOST_LOG_TRIVIAL(debug) << "Found connected node at [" << connectedNode->getX() << "," << connectedNode->getY() << "]";

		if (!(connectedNode->canVisit())) {
			BOOST_LOG_TRIVIAL(debug) << "Connected node may not be visited, aborting...]";
			continue;
		}

		if (currentPath.size() < connectedNode->getMinDist()) {
			BOOST_LOG_TRIVIAL(debug) << "New shortest path to node found. Previous shortest path: " << connectedNode->getPath().size() << ", new shortest path: " << currentPath.size();
			connectedNode->setPath(currentPath);
		}

		pushToQueueIfNotGoal(connectedNode);
		updateGrid(connectedNode);
	}
}

void Pathfinding::pushToQueueIfNotGoal(Node* connectedNode) {
	if (&(*connectedNode) == &(*goal)) {
		BOOST_LOG_TRIVIAL(info) << "Path to the goal found!";
	} else {
		queue.push_back(connectedNode);
		BOOST_LOG_TRIVIAL(debug) << "Connected node is not the goal, adding to queue";
	}
}

void Pathfinding::updateGui(Node* currentNode) {
	if (currentNode->getStatus() == TileStatus::OPEN) {
		grid.setValue(currentNode->getX(), currentNode->getY(), TileStatus::CAN_NOT_VISIT);
	}
}

void Pathfinding::updateGrid(Node* connectedNode) {
	if (connectedNode->getStatus() != TileStatus::START && connectedNode->getStatus() != TileStatus::GOAL) {
		grid.setValue(connectedNode->getX(), connectedNode->getY(), TileStatus::VISITED);
	}
}

void Pathfinding::generateNodes() {
	BOOST_LOG_TRIVIAL(trace) << "Running Pathfinding::generateNodes()";

	for (int x = 0; x < grid.getMaxX(); ++x) {
		for (int y = 0; y < grid.getMaxY(); ++y) {

			vector<Coordinate> connections = generateConnections(x, y);
			Node node{ x, y, connections, grid.getValue(x, y) };
			nodes[x][y] = node;
		}
	}
	BOOST_LOG_TRIVIAL(trace) << "Finishing Pathfinding::generateNodes()";
}

vector<Coordinate> Pathfinding::generateConnections(int x, int y) {
	BOOST_LOG_TRIVIAL(debug) << "Generating connections for node [" << x << ", " << y <<"]";
	vector<Coordinate> connections;
	
	createConnectionIfGridNotBlocked(connections, (x - 1), y);
	createConnectionIfGridNotBlocked(connections, (x + 1), y);
	createConnectionIfGridNotBlocked(connections, x, (y - 1));
	createConnectionIfGridNotBlocked(connections, x, (y + 1));
	BOOST_LOG_TRIVIAL(debug) << "Number of connections for node generated = " << connections.size();
	return connections;
}

void Pathfinding::createConnectionIfGridNotBlocked(vector<Coordinate>& connections, int x, int y) {
	if (!grid.isBlocked(x, y)) {
		BOOST_LOG_TRIVIAL(debug) << "Generating connection at = [" << x << ", " << y << "]";
		Coordinate con{ x, y };
		connections.push_back(con);
	}
}

void Pathfinding::drawPath(Node* goal) {
	vector<Coordinate> path = goal->getPath();
	for (auto it = path.begin()+1; it < path.end(); ++it) {	//Start drawing from path.begin()+1, as path.begin() will always be the start. We don't wanna paint over it
		Coordinate co = *it;
		BOOST_LOG_TRIVIAL(info) << "Path: [" << co.getX() << "," << co.getY() << "]";
		grid.setValue(it->getX(), it->getY(), TileStatus::PATH);
	}
}

Node* Pathfinding::getNode(int x, int y) {
	return &(nodes[x][y]);
}