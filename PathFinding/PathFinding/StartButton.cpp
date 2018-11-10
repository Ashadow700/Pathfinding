#include "pch.h"
#include "StartButton.h"
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using std::cout;
using std::endl;

StartButton::StartButton(RenderWindow& w, Grid& grid, Pathfinding& pathfinding, int origoXPos, int origoYPos, int width, int height) :
	w(w),
	grid(grid),
	pathfinding(pathfinding),
	origoXPos(origoXPos),
	origoYPos(origoYPos),
	width(width),
	height(height) {

	BOOST_LOG_TRIVIAL(trace) << "Start button constructor running";
}

StartButton::~StartButton() {
}

void StartButton::buttonPressed() {
	BOOST_LOG_TRIVIAL(info) << "Start Button pressed" << endl;
	grid.cleanIfNeeded();
	grid.deselectAll();
	pathfinding.execute();
}

void StartButton::drawButton() {
	RectangleShape rectangle(Vector2f(width, height));
	rectangle.setPosition(origoXPos, origoYPos);
	sf::Color color(0, 0, 0);
	rectangle.setFillColor(color);
	w.draw(rectangle);

	sf::Font font;
	if (!font.loadFromFile("Roboto-Bold.ttf")) {
		BOOST_LOG_TRIVIAL(error) << "Failed to load font. Button text will not be displayed Check that \"Roboto-Bold.ttf\" is available in project directory";
		return;
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setString("Start");
	text.setFillColor(Color::White);
	text.setPosition(Vector2f(origoXPos + width/4, origoYPos));
	w.draw(text);
}

bool StartButton::onObject(int xPos, int yPos) {
	return (origoXPos <= xPos && xPos <= origoXPos + width && origoYPos <= yPos && yPos <= origoYPos + height);
}
