#pragma once

#include "Figure.h"

class Knight : public Figure {

public:
	Knight(FigureColor Color);

	virtual FigureType getType() const override { return KNIGHT; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) override;
private:


};