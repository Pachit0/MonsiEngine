#include "Knight.h"

Knight::Knight(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> Knight::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	int dir = (getColor() == WHITE) ? 1 : -1;

	if (!board[y + dir][x])
		moves.push_back({ x, y + dir });

	if (board[y + dir][x + 1])
		moves.push_back({ x + 1, y + dir });

	if (board[y + dir][x - 1])
		moves.push_back({ x - 1, y + dir });

	return moves;
}