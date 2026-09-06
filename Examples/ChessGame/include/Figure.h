#pragma once

#include <vector>

enum FigureColor {
	NULL_COLOR,
	WHITE,
	BLACK,
};

enum FigureType {
	NULL_TYPE,
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

struct MovePoint {
	int x;
	int y;
};

class Figure {
public:

	Figure(FigureColor color);

	FigureColor getColor() const { return m_Color; }
	int getIsUnderAttack() const { return m_IsUnderAttack; }
	void setIsUnderAttack(int IsUnderAttack) { m_IsUnderAttack += IsUnderAttack; }


	virtual FigureType getType() const = 0;
	virtual std::vector<MovePoint> move(int x, int y, Figure* board[8][8]) = 0;
	void setFirstMove() { m_FirstMove = false; }
	bool getFirstMove() { return m_FirstMove; }
	void setEnPassant(bool setEnPassant) { m_EnPassant = setEnPassant; }
	bool getEnPassant() { return m_EnPassant; }
protected:
	FigureColor m_Color;

	//since board is made from array of figures it must hold info is the square under attack;
	int m_IsUnderAttack;
	bool m_FirstMove;
	bool m_EnPassant;
};