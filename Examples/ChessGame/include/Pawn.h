#pragma once

#include "Figure.h"

class Pawn : public Figure {

public:
	Pawn(FigureColor Color);

	virtual FigureType getType() const override { return PAWN; }
	std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) override;

	void setFirstMove() { m_FirstMove = false; }
private:
	bool m_FirstMove;

};