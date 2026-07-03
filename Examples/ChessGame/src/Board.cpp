#include "ChessCore.h"
#include "ChessGame.h"
#include "Board.h"
#include "NullFigure.h"
#include "Pawn.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
	
Board::Board() : m_CurrentTurn(WHITE)
{
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			m_Board[y][x] = new NullFigure(NULL_COLOR);

	m_Board[0][0] = new Rook(WHITE);
	m_Board[0][1] = new Knight(WHITE);
	m_Board[0][2] = new Bishop(WHITE);
	m_Board[0][3] = new Queen(WHITE);
	m_Board[0][4] = new King(WHITE);
	m_Board[0][5] = new Bishop(WHITE);
	m_Board[0][6] = new Knight(WHITE);
	m_Board[0][7] = new Rook(WHITE);

	for (int x = 0; x < 8; x++)
		m_Board[1][x] = new Pawn(WHITE);

	for (int x = 0; x < 8; x++)
		m_Board[6][x] = new Pawn(BLACK);

	m_Board[7][0] = new Rook(BLACK);
	m_Board[7][1] = new Knight(BLACK);
	m_Board[7][2] = new Bishop(BLACK);
	m_Board[7][3] = new Queen(BLACK);
	m_Board[7][4] = new King(BLACK);
	m_Board[7][5] = new Bishop(BLACK);
	m_Board[7][6] = new Knight(BLACK);
	m_Board[7][7] = new Rook(BLACK);
	WKingPtr = m_Board[0][4];
	BKingPtr = m_Board[7][4];
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (m_Board[y][x])
			m_Board[y][x]->move(x, y, m_Board);

		}
	}
}

Board::~Board()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			delete m_Board[y][x];
			m_Board[y][x] = nullptr;
		}
	}
}

bool Board::move(Pos from, Pos to)
{



	if (from.x < 0 || from.x > 7 || from.y < 0 || from.y > 7)
		return false;

	if (to.x < 0 || to.x > 7 || to.y < 0 || to.y > 7)
		return false;

	if (!m_Board[from.y][from.x])
		return false;

	Figure* piece = m_Board[from.y][from.x];

	auto moves = piece->move(from.x, from.y, m_Board);

	bool valid = false;

	for (auto& m : moves)
	{
		if (m.x == to.x && m.y == to.y)
		{
			valid = true;
			break;
		}
	}

	if (!valid)
		return false;

	//if (m_Board[to.y][to.x]) {
	//	if (piece->getColor() == m_Board[to.y][to.x]->getColor()) {
	//		return false;
	//	}
	//}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			m_Board[y][x]->setIsUnderAttack(-m_Board[y][x]->getIsUnderAttack());

		}
	}

	Figure* temp = m_Board[to.y][to.x];
	m_Board[to.y][to.x] = piece;
	m_Board[from.y][from.x] = new NullFigure(NULL_COLOR);

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (m_Board[y][x] != piece)
				m_Board[y][x]->move(x, y, m_Board);

		}
	}

	m_Board[from.y][from.x]->move(to.x, to.y, m_Board);

	if (piece->getColor() == m_CurrentTurn) {
		if (piece->getColor() == WKingPtr->getColor() && (WKingPtr->getIsUnderAttack() == BLACK || WKingPtr->getIsUnderAttack() > BLACK)) {
			delete m_Board[from.y][from.x];
			m_Board[from.y][from.x] = piece;
			m_Board[to.y][to.x] = temp;
			return false;
		}
		if (piece->getColor() == BKingPtr->getColor() && (BKingPtr->getIsUnderAttack() == WHITE || BKingPtr->getIsUnderAttack() > BLACK)) {
			delete m_Board[from.y][from.x];
			m_Board[from.y][from.x] = piece;
			m_Board[to.y][to.x] = temp;
			return false;
		}
	}

	delete m_Board[from.y][from.x];
	m_Board[from.y][from.x] = piece;
	m_Board[to.y][to.x] = temp;

	// capture
	delete m_Board[to.y][to.x];


	// move piece
	
	m_Board[to.y][to.x] = piece;
	m_Board[from.y][from.x] = new NullFigure(NULL_COLOR);

	//set whatever squares are under attack with move method

	//reevaluate the board
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			m_Board[y][x]->setIsUnderAttack(-m_Board[y][x]->getIsUnderAttack());
				
		}
	}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (m_Board[y][x]->getType())
				m_Board[y][x]->move(x, y, m_Board);
		}
	}

	m_CurrentTurn = (m_CurrentTurn == WHITE) ? BLACK : WHITE;

	return true;
}