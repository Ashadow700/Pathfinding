#include "pch.h"
#include "GuiHandler.h"
#include "Grid.h"
#include "Pathfinding.h"
#include "StartButton.h"
#include "TileStatus.h"
#include "GridStatus.h"
#include <iostream>
#include <SFML/Graphics.hpp>


GuiHandler::GuiHandler(sf::RenderWindow& w, Grid& grid, Pathfinding& pathFinding, StartButton& startButton) :
	w(w),
	grid(grid),
	pathFinding(pathFinding),
	startButton(startButton) {
}

GuiHandler::~GuiHandler()
{
}

void GuiHandler::init() {
	
	while (w.isOpen())
	{
		BOOST_LOG_TRIVIAL(trace) << "Starting new refresh cycle";
		w.clear();
		drawBackground();
		startButton.drawButton();
		grid.drawGrid();

		sf::Event event;
		while (w.pollEvent(event)) {

			if (event.type == sf::Event::MouseButtonReleased && inputAllowed) {
				sf::Vector2i position = sf::Mouse::getPosition(w);
				BOOST_LOG_TRIVIAL(debug) << "Mouse released at [" << position.x << ", " << position.y << "]";

				if (startButton.onObject(position.x, position.y)) {
					inputAllowed = false;
					startButton.buttonPressed();
					inputAllowed = true;
				} else if (grid.onObject(position.x, position.y)) {
					handleGridClicked(position.x, position.y);
				}

			} else if (event.type == sf::Event::Closed) {
				BOOST_LOG_TRIVIAL(info) << "Shutting down";
				w.close();
			}

		}
		w.display();
	}
}

void GuiHandler::handleGridClicked(int posX, int posY) {
	Coordinate co = grid.getCoordinateAtMousePos(posX, posY);

	switch (grid.getGridStatus()) {
	case GridStatus::OPEN:
		handleGridOpen(co);
		break;
	case GridStatus::START_SELECTED:
		grid.setStart(co.getX(), co.getY());
		break;
	case GridStatus::GOAL_SELECTED:
		grid.setGoal(co.getX(), co.getY());
		break;
	}

	grid.cleanIfNeeded();
	grid.flipValue(co.getX(), co.getY());
}

void GuiHandler::handleGridOpen(Coordinate& co) {
	if (grid.isStart(co.getX(), co.getY())) {
		grid.selectStart();
	}
	else if (grid.isGoal(co.getX(), co.getY())) {
		grid.selectGoal();
	}
}

void GuiHandler::drawBackground() {
	RectangleShape rectangle(Vector2f(w.getSize().x, w.getSize().y));
	rectangle.setPosition(0, 0);
	rectangle.setFillColor(sf::Color::White);
	w.draw(rectangle);
}

bool GuiHandler::inputAllowed = true;

void GuiHandler::setInputAllowed(bool status) {
	inputAllowed = status;
}



