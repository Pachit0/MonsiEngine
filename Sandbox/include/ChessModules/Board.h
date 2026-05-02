#pragma once

#include "Figure.h"

class Board {

public:
	Board();

//	bool move(char x1, char y1, char x2, char y2);
	bool move(const char* x1, const char* y1, const char* x2, const char* y2);
//	Figure* getIsOccupied(char x1, char y1);
	Figure* getIsOccupied(int newCoordinateX, int newCoordinateY);

private:
	Figure* board[64];
};