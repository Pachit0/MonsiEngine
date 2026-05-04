#include "Queen.h"

Queen::Queen(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> Queen::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	for (int i = x + 1; i < 8; i++) {
		if (board[y][i]) {
			if (board[y][i]->getColor() != getColor())
				moves.push_back({ i, y });
			break;
		}
		moves.push_back({ i, y });
	}

	for (int i = x - 1; i >= 0; i--) {
		if (board[y][i]) {
			if (board[y][i]->getColor() != getColor())
				moves.push_back({ i, y });
			break;
		}
		moves.push_back({ i, y });
	}

	for (int i = y + 1; i < 8; i++) {
		if (board[i][x]) {
			if (board[i][x]->getColor() != getColor())
				moves.push_back({ x, i });
			break;
		}
		moves.push_back({ x, i });
	}

	for (int i = y - 1; i >= 0; i--) {
		if (board[i][x]) {
			if (board[i][x]->getColor() != getColor())
				moves.push_back({ x, i });
			break;
		}
		moves.push_back({ x, i });
	}

	return moves;
}