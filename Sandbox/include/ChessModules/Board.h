#include "ChessStdafx.h"

class Board {
	
	Figure *board[64];
	
	public:

	Board();
	bool move(char x1, char y1, char x2, char y2);
  Figure *getIsOccupied(char x1, char y1);
};