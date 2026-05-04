#include "ChessCore.h"

#include "Board.h"

#include "Pawn.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
	
Board::Board()
{
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			m_Board[y][x] = nullptr;

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

	if (m_Board[to.y][to.x]) {
		if (piece->getColor() == m_Board[to.y][to.x]->getColor()) {
			return false;
		}
	}

	// capture
	delete m_Board[to.y][to.x];

	// move piece
	m_Board[to.y][to.x] = piece;
	m_Board[from.y][from.x] = nullptr;

	return true;
}