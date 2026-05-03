#pragma once

#include "ChessModules/Figure.h"

class Pawn : public Figure {

public:
	Pawn(FigureColor Color);
	int** move(int coordinateX, int coordinateY) override;
private:


};