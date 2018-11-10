#pragma once
class TileStatus
{
public:
	static const int OUT_OF_BOUNDS = -1;
	static const int OPEN = 0;
	static const int BLOCKED = 1;
	static const int VISITED = 2;
	static const int CAN_NOT_VISIT = 3;
	static const int START = 4;
	static const int SELECTED_START = 5;
	static const int GOAL = 6;
	static const int SELECTED_GOAL = 7;
	static const int PATH = 8;
};
