#pragma once

#include "Figure.h"

class Board {

public:
	Board();
	bool move(const char* x1, const char* y1, const char* x2, const char* y2);

private:
	Figure* board[64];
	Figure* whiteKingSquare;
	Figure* blackKingSquare;
};