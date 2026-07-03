#include "Knight.h"

Knight::Knight(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> Knight::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;


	// up 2 - right 1
	if (y + 2 < 8 && x + 1 < 8) {
		board[y + 2][x + 1]->setIsUnderAttack(board[y + 2][x + 1]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y + 2][x + 1]->getColor() != getColor())
			moves.push_back({ x + 1, y + 2 });
	}

	// up 1 - right 2
	if (y + 1 < 8 && x + 2 < 8) {
		board[y + 1][x + 2]->setIsUnderAttack(board[y + 1][x + 2]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y + 1][x + 2]->getColor() != getColor())
			moves.push_back({ x + 2, y + 1 });
	}

	// down 2 - left 1
	if (y - 2 >= 0 && x - 1 >= 0) {
		board[y - 2][x - 1]->setIsUnderAttack(board[y - 2][x - 1]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y - 2][x - 1]->getColor() != getColor())
			moves.push_back({ x - 1, y - 2 });
	}

	// down 1 - left 2
	if (y - 1 >= 0 && x - 2 >= 0) {
		board[y - 1][x - 2]->setIsUnderAttack(board[y - 1][x - 2]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y - 1][x - 2]->getColor() != getColor())
			moves.push_back({ x - 2, y - 1 });
	}

	// up 2 - left 1
	if (y + 2 < 8 && x - 1 >= 0) {
		board[y + 2][x - 1]->setIsUnderAttack(board[y + 2][x - 1]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y + 2][x - 1]->getColor() != getColor())
			moves.push_back({ x - 1, y + 2 });
	}

	// up 1 - left 2
	if (y + 1 < 8 && x - 2 >= 0) {
		board[y + 1][x - 2]->setIsUnderAttack(board[y + 1][x - 2]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y + 1][x - 2]->getColor() != getColor())
			moves.push_back({ x - 2, y + 1 });
	}

	// down 2 - right 1
	if (y - 2 >= 0 && x + 1 < 8) {
		board[y - 2][x + 1]->setIsUnderAttack(board[y - 2][x + 1]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y - 2][x + 1]->getColor() != getColor())
			moves.push_back({ x + 1, y - 2 });
	}

	// down 1 - right 2
	if (y - 1 >= 0 && x + 2 < 8) {
		board[y - 1][x + 2]->setIsUnderAttack(board[y - 1][x + 2]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y - 1][x + 2]->getColor() != getColor())
			moves.push_back({ x + 2, y - 1 });
	}

	return moves;
}