#include "NullFigure.h"

NullFigure::NullFigure(FigureColor Color) : Figure(Color)
{

}

std::vector<MovePoint> NullFigure::move(int x, int y, Figure* board[8][8]) { return {}; }