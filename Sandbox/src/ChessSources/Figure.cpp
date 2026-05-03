#include "ChessModules/ChessCore.h"
#include "ChessModules/Figure.h"

Figure::Figure()
{
	m_Color = NULL_COLOR;
}

Figure::Figure(FigureColor color) {
		m_Color = color;
}

