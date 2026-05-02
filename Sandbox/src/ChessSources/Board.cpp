#include "ChessStdafx.h"

Board *boardPtr;
	
Board::Board() {
//standart game - standart starting squares, no additional pieces, no handicaps
  board[0] = new Rook(true); //x1 y1 //1(this is y) + (1(this is y)-1*8)-1 formula for coordinates
  board[1] = new Knight(true); // x2 y1
  board[2] = new Bishop(true); // x3 y1
  board[3] = new Queen(true);
  board[4] = new King(true);
  board[5] = new Bishop(true);
  board[6] = new Knight(true);
  board[7] = new Rook(true);
	int i = 8;
	for (; i<16 ;i++)
    board[i] = new Pawn(true); //x2 y1//1+(2-1)*8-1 = 8
	
	for (; i<48 ;i++)
    board[i] = nullptr;
	
	for (; i<56 ;i++)
    board[i] = new Pawn(false);

  board[56] = new Rook(false);//x8 y1//1+(8-1)*8-1 = 56
  board[57] = new Knight(false);
  board[58] = new Bishop(false);
  board[59] = new Queen(false);
  board[60] = new King(false);
  board[61] = new Bishop(false);
  board[62] = new Knight(false);
  board[63] = new Rook(false);
}
//add aditional constructors for non-standart games


/////////////////////////////////////////////
//move method -
//it checks if command move has syntatically correct coordinates,
//if the chess square the piece will move onto is occupied by piece of the same color, 
//is the king of the same color under check
//is this piece under pin//king is exception
/////////////////////////////////////////////
bool Board::move(const char *x1, const char *y1, const char *x2, const char *y2) {
	if (strlen(x1) != 2 || strlen(y1) != 2 || strlen(x2) != 2 || strlen(y2) != 2) {
		cout << "Invalid command! Too long or too short! Example: x1" << endl;
	  return false;
	}
  if (x1[0] != 'x' || y1[0] != 'y' || x2[0] != 'x' || y2[0] != 'y') {
		cout << "Invalid command! Used different coordinates! You can only use x and y" << endl;
		return false;
	}
	int initialCoordinateX = x1[1]-'0';
	int initialCoordinateY = y1[1]-'0';
	int newCoordinateX = x2[1]-'0';
	int newCoordinateY = y2[1]-'0';
	if (initialCoordinateX > MAX_COORD || initialCoordinateX < MIN_COORD ||
	    initialCoordinateY > MAX_COORD || initialCoordinateY < MIN_COORD ||
	    newCoordinateX > MAX_COORD || newCoordinateX < MIN_COORD ||
			newCoordinateY > MAX_COORD || newCoordinateY < MIN_COORD) {
    cout << "Invalid command! Too big, too small or negative coordinates used! "
    << "You can only type from " << MIN_COORD
    << " to " << MAX_COORD << endl;
		return false;
	}
	
	if(getIsOccupied(newCoordinateX, newCoordinateY) != nullptr) {
		cout << "Invalid command! Occupied square! You can only move to squares that have no figures of your color" << endl;
		return false;
	}
}

Figure *Board::getIsOccupied(int newCoordinateX, int newCoordinateY) {
  Figure &temp = board[newCoordinateX - 1 + (newCoordinateY - 1)*8];
	//we need temp.color to know if its our color
}