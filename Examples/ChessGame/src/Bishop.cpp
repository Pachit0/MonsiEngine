#include "Bishop.h"

Bishop::Bishop(FigureColor Color) : Figure(Color)
{

}

std::vector<MovePoint> Bishop::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	// up-right
	for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++) {
		if (board[j][i]) {
			if (board[j][i]->getColor() != getColor())
				moves.push_back({ i, j });
			break;
		}
		moves.push_back({ i, j });
	}

	// down-left
	for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
		if (board[j][i]) {
			if (board[j][i]->getColor() != getColor())
				moves.push_back({ i, j });
			break;
		}
		moves.push_back({ i, j });
	}

	// up-left
	for (int i = y + 1, j = x - 1; i < 8 && j >= 0; i++, j--) {
		if (board[i][j]) {
			if (board[i][j]->getColor() != getColor())
				moves.push_back({ j, i });
			break;
		}
		moves.push_back({ j, i });
	}

	// down-right
	for (int i = y - 1, j = x + 1; i >= 0 && j < 8; i--, j++) {
		if (board[i][j]) {
			if (board[i][j]->getColor() != getColor())
				moves.push_back({ j, i });
			break;
		}
		moves.push_back({ j, i });
	}

	return moves;
}