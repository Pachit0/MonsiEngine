#pragma once

#include "Figure.h"

class Queen : public Figure {

public:
	Queen(FigureColor Color);

	virtual FigureType getType() const override { return QUEEN; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) override;
private:


};