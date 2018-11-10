#include "pch.h"
#include "Grid.h"
#include "TileStatus.h"
#include "GridStatus.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace std;
using namespace sf;

Grid::Grid(int width, int height, int origoXPos, int origoYPos, int squareSize, int outline, RenderWindow& w) :
	width(width),
	height(height),
	origoXPos(origoXPos),
	origoYPos(origoYPos),
	squareSize(squareSize),
	outline(outline),
	w(w) {
	BOOST_LOG_TRIVIAL(trace) << "Grid constructor running";

	validateProportions(width, height, squareSize);
	maxX = (width / squareSize);
	maxY = (height / squareSize);

	matrix.resize(maxX);
	for (int x = 0; x < maxX; ++x) {
		matrix[x].resize(maxY);
	}

	setStart( maxX / 4, maxY / 2);
	setGoal( (maxX *3 / 4), maxY / 2);
	gridStatus = GridStatus::OPEN;
	needsCleaning = false;
	BOOST_LOG_TRIVIAL(trace) << "Grid constructor finished";
}

Grid::~Grid() {
}

void Grid::validateProportions(int width, int height, int squareSize) const {
	if ( width%squareSize != 0 || height % squareSize != 0) {
		BOOST_LOG_TRIVIAL(fatal) << "Width and height must both be evenly divisible into squareSize, width%squareSize = " << (width % squareSize) <<", height%squareSize = " << (height % squareSize);
		throw std::invalid_argument("Width and height must both be evenly divisible into squareSize!");
	}
}

void Grid::setValue(int x, int y, int value) {
	if (notOutOfBounds(x, y)) {
		matrix[x][y] = value;
	} 
}

void Grid::setValueAtMousePos(int posX, int posY, int value) {
	Coordinate co = convertMousePosToCoordinate(posX, posY);
	setValue(co.getX(), co.getY(), value);
}

Coordinate Grid::getCoordinateAtMousePos(int posX, int posY) {
	return convertMousePosToCoordinate(posX, posY);
}

Coordinate Grid::convertMousePosToCoordinate(int posX, int posY) {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::convertMousePosToCoordinate()";
	posX = posX - origoXPos;
	posY = posY - origoYPos;
	posX -= (posX % squareSize);
	posY -= (posY % squareSize);
	posX = posX / squareSize;
	posY = posY / squareSize;
	BOOST_LOG_TRIVIAL(trace) << "Finishing Grid::convertMousePosToCoordinate()";
	return Coordinate{ posX, posY };
}

Coordinate Grid::getStart() const {
	return start;
}

Coordinate Grid::getGoal() const {
	return goal;
}

void Grid::setStart(int x, int y) {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::setStart()";
	if (canSelect(x, y)) {
		setValue(x, y, TileStatus::START);
		setValue(start.getX(), start.getY(), TileStatus::OPEN);
		start = Coordinate{ x, y };
		gridStatus = GridStatus::OPEN;
		BOOST_LOG_TRIVIAL(info) << "Setting [ " << x << ", " << y << "] to new Start";
	}
}

void Grid::setGoal(int x, int y) {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::setGoal()";
	if (canSelect(x, y)) {
		setValue(x, y, TileStatus::GOAL);
		setValue(goal.getX(), goal.getY(), TileStatus::OPEN);
		goal = Coordinate{ x, y };
		gridStatus = GridStatus::OPEN;
		BOOST_LOG_TRIVIAL(info) << "Setting [ " << x << ", " << y << "] to new goal";
	}
}

void Grid::selectStart() {
	setValue(getStart().getX(), getStart().getY(), TileStatus::SELECTED_START);
	gridStatus = GridStatus::START_SELECTED;
	BOOST_LOG_TRIVIAL(info) << "Start selected";
}

void Grid::selectGoal() {
	setValue(getGoal().getX(), getGoal().getY(), TileStatus::SELECTED_GOAL);
	gridStatus = GridStatus::GOAL_SELECTED;
	BOOST_LOG_TRIVIAL(info) << "Goal selected";
}

bool Grid::isStart(int x, int y) const {
	return (x == start.getX() && y == start.getY());
}

bool Grid::isGoal(int x, int y) const {
	return (x == goal.getX() && y == goal.getY());
}

void Grid::deselectAll() {
	BOOST_LOG_TRIVIAL(trace) << "Grid::deselectAll()";
	setValue(getStart().getX(), getStart().getY(), TileStatus::START);
	setValue(getGoal().getX(), getGoal().getY(), TileStatus::GOAL);
	gridStatus = GridStatus::OPEN;
}

bool Grid::canSelect(int x, int y) const {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::canSelect(" << x << ", " << y << ")";
	if (notOutOfBounds(x, y)) {
		return (matrix[x][y] == TileStatus::OPEN || matrix[x][y] == TileStatus::BLOCKED);
	} else {
		return false;
	}
}

int Grid::getGridStatus() const {
	return gridStatus;
}

bool Grid::notOutOfBounds(int x, int y) const {
	if ((0 <= x && x < maxX && 0 <= y && y < maxY)) {	//would have prefered to simply return (0 <= x  && x < maxX && 0 <= y && y < maxY), but then I could not include warning log
		return true;
	} else {
		BOOST_LOG_TRIVIAL(warning) << "Requested position [" << x << ", " << y << "] is out ouf bounds, returning false";
		return false;
	}
}

int Grid::getValue(int x, int y) const {
	if (notOutOfBounds(x, y)) {
		BOOST_LOG_TRIVIAL(debug) << "At Grid::getValue, returning " << matrix[x][y];
		return(matrix[x][y]);
	}
	else {
		return TileStatus::OUT_OF_BOUNDS;
	}
}

void Grid::flipValue(int x, int y) {
	BOOST_LOG_TRIVIAL(trace) << "Flipping value at position [" << x << ", " << y << "]";
	if (notOutOfBounds(x, y) ) {
		if (matrix[x][y] == TileStatus::OPEN || matrix[x][y] == TileStatus::PATH || matrix[x][y] == TileStatus::VISITED) {
			matrix[x][y] = TileStatus::BLOCKED;
		}
		else if (matrix[x][y] == TileStatus::BLOCKED) {
			matrix[x][y] = TileStatus::OPEN;
		}
	}
}

int Grid::getMaxX() const {
	return maxX;
}

int Grid::getMaxY() const {
	return maxY;
}

int Grid::getSquareSize() {
	return squareSize;
}

bool Grid::onObject(int xPos, int yPos) {
	return (origoXPos <= xPos && xPos <= origoXPos + width && origoYPos <= yPos && yPos <= origoYPos + height);
}

bool Grid::isBlocked(int x, int y) const {
	return (getValue(x, y) == TileStatus::BLOCKED || getValue(x, y) == TileStatus::OUT_OF_BOUNDS);
}

void Grid::drawGrid() {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::drawGrid()";
	drawBackground(sf::Color::Black, outline);
	for (int x = 0; x < maxX; ++x) {
		for (int y = 0; y < maxY; ++y) {
			if (matrix[x][y] == TileStatus::BLOCKED) {
				drawSquare(x, y, Color::White );		
			}
			else if (matrix[x][y] == TileStatus::VISITED) {
				drawSquare(x, y, Color(19, 123, 213));
			}
			else if (matrix[x][y] == TileStatus::CAN_NOT_VISIT) {
				drawSquare(x, y, Color(131, 183, 229));
			}
			else if (matrix[x][y] == TileStatus::START) {
				drawSquare(x, y, Color::Green);
			}
			else if (matrix[x][y] == TileStatus::GOAL) {
				drawSquare(x, y, Color::Red);
			}
			else if (matrix[x][y] == TileStatus::SELECTED_START) {
				drawSquare(x, y, Color(104, 189, 110));
			}
			else if (matrix[x][y] == TileStatus::SELECTED_GOAL) {
				drawSquare(x, y, Color(255, 92, 92));
			}
			else if (matrix[x][y] == TileStatus::PATH) {
				drawSquare(x, y, Color::Yellow);
			}

		}
	}
	BOOST_LOG_TRIVIAL(trace) << "Finished Grid::drawGrid()";
}

void Grid::drawBackground(sf::Color color, int outline) const {
	RectangleShape rectangle(Vector2f(width + 2*outline, height + 2*outline));
	rectangle.setPosition(origoXPos - outline, origoYPos - outline);
	rectangle.setFillColor(color);
	w.draw(rectangle);
}

void Grid::drawSquare(int row, int col) const {
	drawSquare(row, col, sf::Color::White);
}

void Grid::drawSquare(int row, int col, sf::Color color) const {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::drawSquare()";
	RectangleShape rectangle(Vector2f(squareSize, squareSize));
	rectangle.setPosition(origoXPos + row*squareSize, origoYPos + col*squareSize);
	rectangle.setFillColor(color);
	w.draw(rectangle);
	BOOST_LOG_TRIVIAL(trace) << "Finished Grid::drawSquare()";
}

void Grid::cleanIfNeeded() {
	if (needsCleaning) {
		clean();
	}
}

void Grid::clean() {
	BOOST_LOG_TRIVIAL(trace) << "Running Grid::clean()";

	for (int x = 0; x < maxX; ++x) {
		for (int y = 0; y < maxY; ++y) {
			if (matrix[x][y] == TileStatus::CAN_NOT_VISIT || matrix[x][y] == TileStatus::VISITED || matrix[x][y] == TileStatus::PATH) {
				matrix[x][y] = TileStatus::OPEN;
			}
		}
	}
	drawGrid();
	w.display();
	setNeedsCleaning(false);
	BOOST_LOG_TRIVIAL(debug) << "Finished Grid::clean()";
}

bool Grid::getNeedsCleaning() const {
	return needsCleaning;
}

void Grid::setNeedsCleaning(bool status) {
	needsCleaning = status;
}