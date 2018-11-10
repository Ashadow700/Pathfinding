#include "pch.h"
#include "Coordinate.h"

Coordinate::Coordinate(int x, int y) : 
	x(x),
	y(y) {
}

Coordinate::Coordinate() {
}

Coordinate::~Coordinate()
{
}

int Coordinate::getX() const { 
	return x;
}

int Coordinate::getY() const {
	return y;
}