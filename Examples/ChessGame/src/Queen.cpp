#include "Queen.h"

Queen::Queen(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> Queen::move(int x, int y, Figure* board[8][8]) {
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

	// up-right
	for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++) {
		board[j][i]->setIsUnderAttack(board[j][i]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[j][i]->getColor()) {
			if (board[j][i]->getColor() != getColor())
				moves.push_back({ i, j });
			break;
		}
		moves.push_back({ i, j });
	}

	// down-left
	for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
		board[j][i]->setIsUnderAttack(board[j][i]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[j][i]->getColor()) {
			if (board[j][i]->getColor() != getColor())
				moves.push_back({ i, j });
			break;
		}
		moves.push_back({ i, j });
	}

	// up-left
	for (int i = y + 1, j = x - 1; i < 8 && j >= 0; i++, j--) {
		board[i][j]->setIsUnderAttack(board[i][j]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[i][j]->getColor()) {
			if (board[i][j]->getColor() != getColor())
				moves.push_back({ j, i });
			break;
		}
		moves.push_back({ j, i });
	}

	// down_right
	for (int i = y - 1, j = x + 1; i >= 0 && j < 8; i--, j++) {
		board[i][j]->setIsUnderAttack(board[i][j]->getIsUnderAttack() == getColor() ? 0 : getColor());
		if (board[i][j]->getColor()) {
			if (board[i][j]->getColor() != getColor())
				moves.push_back({ j, i });
			break;
		}
		moves.push_back({ j, i });
	}

	return moves;
}