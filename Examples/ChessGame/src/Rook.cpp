#include "Rook.h"

Rook::Rook(FigureColor Color) : Figure(Color)
{

}

std::vector<MovePoint> Rook::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	// right
	for (int i = x + 1; i < 8; i++) {
		board[y][i]->setIsUnderAttack(board[y][i]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y][i]->getColor()) {
			if (board[y][i]->getColor() != getColor())
				moves.push_back({ i, y });
			break;
		}
		moves.push_back({ i, y });
	}

	// left
	for (int i = x - 1; i >= 0; i--) {
		board[y][i]->setIsUnderAttack(board[y][i]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[y][i]->getColor()) {
			if (board[y][i]->getColor() != getColor())
				moves.push_back({ i, y });
			break;
		}
		moves.push_back({ i, y });
	}

	// up
	for (int i = y + 1; i < 8; i++) {
		board[i][x]->setIsUnderAttack(board[i][x]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[i][x]->getColor()) {
			if (board[i][x]->getColor() != getColor())
				moves.push_back({ x, i });
			break;
		}
		moves.push_back({ x, i });
	}

	// down
	for (int i = y - 1; i >= 0; i--) {
		board[i][x]->setIsUnderAttack(board[i][x]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[i][x]->getColor()) {
			if (board[i][x]->getColor() != getColor())
				moves.push_back({ x, i });
			break;
		}
		moves.push_back({ x, i });
	}

	return moves;
}