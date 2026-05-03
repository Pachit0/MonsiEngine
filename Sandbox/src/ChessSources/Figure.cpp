#include "ChessModules/ChessCore.h"
#include "ChessModules/Figure.h"

Figure::Figure()
{
	m_Color = NULL_COLOR;
	m_IsUnderPin = false;
	m_IsUnderCheck = false;
}

Figure::Figure(FigureColor color) {
		m_Color = color;
		m_IsUnderPin = false;
		m_IsUnderCheck = false;
}

FigureColor Figure::getColor()
{
	return m_Color;
}

void Figure::printStats()
{

}

//bool Figure::move() {//to be cleaned

//}

bool Figure::getIsUnderCheck()
{
	return false;
}

bool Figure::getIsUnderPin()
{
	return false;
}

