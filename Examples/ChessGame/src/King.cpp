#include "King.h"

King::King(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> King::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	// up 1
	if (y + 1 < 8) {
		//set the square to be under attack by the color of the king, even if he can't move on it
		board[y + 1][x]->setIsUnderAttack(board[y + 1][x]->getIsUnderAttack() == getColor() ? 0 : getColor());

		//king must check if a square is under attack unlike other pieces
		if (!board[y + 1][x]->getIsUnderAttack() || board[y + 1][x]->getIsUnderAttack() == getColor())
				if (board[y + 1][x]->getColor() != getColor())
					moves.push_back({ x, y + 1 });
	}
	// left 1
	if (x - 1 >= 0) {
		board[y][x - 1]->setIsUnderAttack(board[y][x - 1]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y][x - 1]->getIsUnderAttack() || board[y][x - 1]->getIsUnderAttack() == getColor())
				if (board[y][x - 1]->getColor() != getColor())
					moves.push_back({ x - 1, y });
	}

	// right 1
	if (x + 1 < 8) {
		board[y][x + 1]->setIsUnderAttack(board[y][x + 1]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y][x + 1]->getIsUnderAttack() || board[y][x + 1]->getIsUnderAttack() == getColor())
				if (board[y][x + 1]->getColor() != getColor())
					moves.push_back({ x + 1, y });
	}

	// down 1
	if (y - 1 >= 0) {
		board[y - 1][x]->setIsUnderAttack(board[y - 1][x]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y - 1][x]->getIsUnderAttack() || board[y - 1][x]->getIsUnderAttack() == getColor())
				if (board[y - 1][x]->getColor() != getColor())
					moves.push_back({ x, y - 1 });
	}

	// up 1 - right 1
	if (y + 1 < 8 && x + 1 < 8) {
		board[y + 1][x + 1]->setIsUnderAttack(board[y + 1][x + 1]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y + 1][x + 1]->getIsUnderAttack() || board[y + 1][x + 1]->getIsUnderAttack() == getColor())
				if (board[y + 1][x + 1]->getColor() != getColor())
					moves.push_back({ x + 1, y + 1 });
	}

	// up 1 - left 1
	if (y + 1 < 8 && x - 1 >= 0) {
		board[y + 1][x - 1]->setIsUnderAttack(board[y + 1][x - 1]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y + 1][x - 1]->getIsUnderAttack() || board[y + 1][x - 1]->getIsUnderAttack() == getColor())
				if (board[y + 1][x - 1]->getColor() != getColor())
					moves.push_back({ x - 1, y + 1 });
	}

	// down 1 - left 1
	if (y - 1 >= 0 && x - 1 >= 0) {
		board[y - 1][x - 1]->setIsUnderAttack(board[y - 1][x - 1]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y - 1][x - 1]->getIsUnderAttack() || board[y - 1][x - 1]->getIsUnderAttack() == getColor())
				if (board[y - 1][x - 1]->getColor() != getColor())
					moves.push_back({ x - 1, y - 1 });
	}

	// down 1 - right 1
	if (y - 1 >= 0 && x + 1 < 8) {
		board[y - 1][x + 1]->setIsUnderAttack(board[y - 1][x + 1]->getIsUnderAttack() == getColor() ? 0 : getColor());

		if (!board[y - 1][x + 1]->getIsUnderAttack() || board[y - 1][x + 1]->getIsUnderAttack() == getColor())
				if (board[y - 1][x + 1]->getColor() != getColor())
					moves.push_back({ x + 1, y - 1 });
	}
	return moves;
}