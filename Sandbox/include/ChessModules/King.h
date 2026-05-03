#pragma once

#include "ChessModules/Figure.h"

class King : public Figure {

public:
	King(FigureColor Color);
	int** move(int coordinateX, int coordinateY) override;
private:
	bool m_IsUnderCheck;

};