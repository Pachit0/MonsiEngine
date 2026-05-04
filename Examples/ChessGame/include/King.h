#pragma once

#include "Figure.h"

class King : public Figure {

public:
	King(FigureColor Color);

	virtual FigureType getType() const override { return KING; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) override;
private:
	bool m_IsUnderCheck;

};