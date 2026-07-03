#pragma once

#include "Figure.h"

class NullFigure : public Figure {

public:
	NullFigure(FigureColor Color);

	virtual FigureType getType() const override { return NULL_TYPE; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]);
private:


};