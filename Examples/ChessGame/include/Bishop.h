#pragma once

#include "Figure.h"

class Bishop : public Figure {
public:
	Bishop(FigureColor Color);

	virtual FigureType getType() const override { return BISHOP; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) override;
private:

};