#pragma once
#include "Grid.h"
#include "Pathfinding.h"

class StartButton
{
public:
	StartButton(RenderWindow& w, Grid& grid, Pathfinding& pathfinding, int origoXPos, int origoYPos, int width, int height);
	~StartButton();

	void buttonPressed();
	bool onObject(int xPos, int yPos);
	void drawButton();
	
private:
	RenderWindow& w;
	Grid& grid;
	Pathfinding& pathfinding;
	int origoXPos;
	int origoYPos;
	int width;
	int height;
};

