#include "pch.h"
#include "Grid.h"
#include "Pathfinding.h"
#include "StartButton.h"
#include "GuiHandler.h"
#include <SFML/Graphics.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

void initLogging() {
	boost::log::core::get()->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::error
	);
}

int main() {
	initLogging();
	sf::RenderWindow w(sf::VideoMode(800, 600), "Pathfinding");
	Grid grid(600, 400, 100, 100, 50, 7, w);
	Pathfinding pathfidning{ grid , w};
	StartButton startButton(w, grid, pathfidning, 350, 20, 100, 30);
	GuiHandler gui(w, grid, pathfidning, startButton);

	gui.init();
	return 0;
}