#include "Pawn.h"

Pawn::Pawn(FigureColor Color) : Figure(Color), m_FirstMove(true){

}

std::vector<MovePoint> Pawn::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	int dir = (getColor() == WHITE) ? 1 : -1;

	// forward 1
	if (y + dir < 8)
		if (!board[y + dir][x]->getType())
			moves.push_back({ x, y + dir });

	// forward 2
	if (y + dir + dir < 8)
		if (!board[y + dir + dir][x]->getType() && !board[y + dir][x]->getType() && m_FirstMove)
			moves.push_back({ x, y + dir + dir });

	// take forward 1 right 1
	if (y + dir < 8 && x + 1 < 8) {
		board[y + dir][x + 1]->setIsUnderAttack(board[y + dir][x + 1]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y + dir][x + 1]->getType())
			moves.push_back({ x + 1, y + dir });
	}
	// take forward 1 left 1
	if (y + dir < 8 && x - 1 < 8) {
		board[y + dir][x - 1]->setIsUnderAttack(board[y + dir][x - 1]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y + dir][x - 1]->getType())
			moves.push_back({ x - 1, y + dir });
	}
	/*Pawn* pawnLeft = dynamic_cast<Pawn*>(board[4][x - 1]);
	Pawn* pawnRight = dynamic_cast<Pawn*>(board[4][x + 1]);

	//en passant
	if (getColor() == WHITE)
		if (y == 4 && 
			(board[4][x - 1]->getType() == PAWN && !pawnLeft->m_FirstMove || 
			 board[4][x + 1]->getType() == PAWN && !pawnRight->m_FirstMove))
			moves.push_back({ x, y + dir });
		else if 


	if (board[y + dir][x - 1]->getType())
		moves.push_back({ x - 1, y + dir });*/

	return moves;
}