#pragma once
#include "Coordinate.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Grid
{

public:
	Grid(int width, int height, int origoXPos, int origoYPos, int squareSize, int outline, RenderWindow& w);
	~Grid();

	int getSquareSize();
	void drawGrid();
	
	void setValue(int x, int y, int value);
	void setValueAtMousePos(int mousePosX, int mousePosY, int value);
	Coordinate getCoordinateAtMousePos(int mousePosX, int mousePosY);
	Coordinate getStart() const;
	Coordinate getGoal() const;
	void setGoal(int x, int y);
	void setStart(int x, int y);
	void selectGoal();
	void selectStart();
	bool isGoal(int x, int y) const;
	bool isStart(int x, int y) const;
	void deselectAll();
	int getGridStatus() const;
	int getValue(int x, int y) const;
	void flipValue(int x, int y);
	int getMaxX() const;
	int getMaxY() const;
	bool onObject(int xPos, int yPos);
	bool isBlocked(int x, int y) const;
	void cleanIfNeeded();
	void clean();
	bool getNeedsCleaning() const;
	void setNeedsCleaning(bool status);


private:
	RenderWindow& w;
	int width;
	int height;
	int origoXPos;
	int origoYPos;
	int squareSize;
	int outline;
	int maxX;
	int maxY;
	Coordinate goal;
	Coordinate start;
	std::vector< std::vector<int> > matrix;
	bool needsCleaning;
	int gridStatus;

	Coordinate convertMousePosToCoordinate(int posX, int posY);
	void validateProportions(int width, int height, int squareSize) const;
	bool notOutOfBounds(int x, int y) const;
	bool canSelect(int x, int y) const;
	void drawSquare(int x, int y) const;
	void drawSquare(int x, int y, sf::Color color) const;
	void drawBackground(sf::Color color, int outline) const;
};