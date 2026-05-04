#include "King.h"

King::King(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> King::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	if (board[y][x])
		moves.push_back({ x, y });

	if (board[y][x + 1])
		moves.push_back({ x + 1, y });

	if (board[y][x - 1])
		moves.push_back({ x - 1, y });

	return moves;
}