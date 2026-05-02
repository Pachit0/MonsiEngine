#include "ChessStdafx.h"

class Figure {
	bool color;//true-White, false-Black
	bool isUnderCheck;
	bool isUnderPin;

	public:
	Figure(bool color);
	virtual void printStats();
	virtual bool move();
	virtual bool getIsUnderCheck();
	virtual bool getIsUnderPin();
};

class Pawn : public Figure {
};

class Rook : public Figure {
};

class Knight : public Figure {
};

class Bishop : public Figure {
};

class Queen : public Bishop, public Rook {
};

class King : public Figure {
};