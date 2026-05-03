#pragma once

#include "ChessModules/Figure.h"

class Bishop : public Figure {
public:
	Bishop(FigureColor Color);
	int** move(int coordinateX, int coordinateY) override;
private:

};