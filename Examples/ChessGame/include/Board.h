#pragma once

#include "Figure.h"
#include <vector>

struct Pos {
	int x, y;
};

class Board {
public:
	Board();

	bool move(Pos from, Pos to);

	Figure* get(int x, int y) { return m_Board[y][x]; }

private:
	Figure* m_Board[8][8] = {};
};