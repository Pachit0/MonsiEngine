#include "ChessCore.h"
#include "Figure.h"

Figure::Figure(FigureColor color) : m_Color(color), m_IsUnderAttack(0), m_FirstMove(true), m_EnPassant(false) {

}

