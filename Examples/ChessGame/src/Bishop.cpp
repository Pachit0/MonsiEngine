#include "Bishop.h"

Bishop::Bishop(FigureColor Color) : Figure(Color)
{

}

std::vector<MovePoint> Bishop::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

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

	// down-right
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