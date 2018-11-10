#pragma once
#include "Grid.h"
#include "Pathfinding.h"
#include "StartButton.h"

class GuiHandler
{
public:
	GuiHandler(sf::RenderWindow& w, Grid& grid, Pathfinding& pathFinding, StartButton& startButton);
	~GuiHandler();

	void init();

	static void setInputAllowed(bool status);

private:
	sf::RenderWindow& w;
	Grid& grid;
	Pathfinding& pathFinding;
	StartButton& startButton;

	void handleGridClicked(int posX, int posY);
	void handleGridOpen(Coordinate& co);
	void drawBackground();

	static bool inputAllowed;
};

