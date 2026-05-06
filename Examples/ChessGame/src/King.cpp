#include "King.h"

King::King(FigureColor Color) : Figure(Color) {

}

std::vector<MovePoint> King::move(int x, int y, Figure* board[8][8]) {
	std::vector<MovePoint> moves;

	// up 1
	if (y + 1 < 8)
		if (board[y + 1][x]) {
			if (board[y + 1][x]->getColor() != getColor())
				moves.push_back({ x, y + 1 });
		}
		else {
			moves.push_back({ x, y + 1 });
		}

	// left 1
	if (x - 1 >= 0)
		if (board[y][x - 1]) {
			if (board[y][x - 1]->getColor() != getColor())
				moves.push_back({ x - 1, y });
		}
		else {
			moves.push_back({ x - 1, y });
		}

	// right 1
	if (x + 1 < 8)
		if (board[y][x + 1]) {
			if (board[y][x + 1]->getColor() != getColor())
				moves.push_back({ x + 1, y });
		}
		else {
			moves.push_back({ x + 1, y });
		}

	// down 1
	if (y - 1 >= 0)
		if (board[y - 1][x]) {
			if (board[y - 1][x]->getColor() != getColor())
				moves.push_back({ x, y - 1 });
		}
		else {
			moves.push_back({ x, y - 1 });
		}

	// up 1 - right 1
	if (y + 1 < 8 && x + 1 < 8)
		if (board[y + 1][x + 1]) {
			if (board[y + 1][x + 1]->getColor() != getColor())
				moves.push_back({ x + 1, y + 1 });
		}
		else {
			moves.push_back({ x + 1, y + 1 });
		}

	// up 1 - left 1
	if (y + 1 < 8 && x - 1 >= 0)
		if (board[y + 1][x - 1]) {
			if (board[y + 1][x - 1]->getColor() != getColor())
				moves.push_back({ x - 1, y + 1 });
		}
		else {
			moves.push_back({ x - 1, y + 1 });
		}

	// down 1 - left 1
	if (y - 1 >= 0 && x - 1 >= 0)
		if (board[y - 1][x - 1]) {
			if (board[y - 1][x - 1]->getColor() != getColor())
				moves.push_back({ x - 1, y - 1 });
		}
		else {
			moves.push_back({ x - 1, y - 1 });
		}

	// down 1 - right 1
	if (y - 1 >= 0 && x + 1 < 8)
		if (board[y - 1][x + 1]) {
			if (board[y - 1][x + 1]->getColor() != getColor())
				moves.push_back({ x + 1, y - 1 });
		}
		else {
			moves.push_back({ x + 1, y - 1 });
		}

	return moves;
}