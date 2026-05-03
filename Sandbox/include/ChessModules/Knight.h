#pragma once

#include "ChessModules/Figure.h"

class Knight : public Figure {

public:
	Knight(FigureColor Color);
	int** move(int coordinateX, int coordinateY) override;
private:


};