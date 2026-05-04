#pragma once

#include <vector>

enum FigureColor {
	WHITE,
	BLACK,
	NULL_COLOR
};

enum FigureType {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING,
	NULL_TYPE
};

struct MovePoint {
	int x;
	int y;
};

class Figure {
public:

	Figure(FigureColor color);

	FigureColor getColor() const { return m_Color; }

	virtual FigureType getType() const = 0;
	virtual std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) = 0;

private:
	FigureColor m_Color;
};