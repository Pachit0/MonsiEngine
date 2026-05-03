#pragma once

#include "ChessModules/Figure.h"

class Queen : public Figure {

public:
	Queen(FigureColor Color);
	int** move(int coordinateX, int coordinateY) override;
private:


};