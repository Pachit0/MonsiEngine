#pragma once

#include "Figure.h"

class Rook : public Figure {

public:
	Rook(FigureColor Color);

	virtual FigureType getType() const override { return ROOK; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) override;

private:


};