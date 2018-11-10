#pragma once
class Coordinate
{
public:
	Coordinate(int x, int y);
	Coordinate();
	~Coordinate();

	int getX() const;
	int getY() const;

private:
	int x;
	int y;
};

