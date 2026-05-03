#pragma once

#include "ChessModules/Figure.h"

class Rook : public Figure {

public:
	Rook(FigureColor Color);
	int **move (int coordinateX, int coordinateY) override;
private:


};