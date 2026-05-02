#include "ChessModules/ChessCore.h"

#include "ChessModules/Board.h"

#include "ChessModules/Pawn.h"
#include "ChessModules/Queen.h"
#include "ChessModules/King.h"
#include "ChessModules/Bishop.h"
#include "ChessModules/Knight.h"
#include "ChessModules/Rook.h"
	
Board::Board() {
//standart game - standart starting squares, no additional pieces, no handicaps
  board[0] = new Rook(FigureColor::WHITE); //x1 y1 //1(this is y) + (1(this is y)-1*8)-1 formula for coordinates
  board[1] = new Knight(FigureColor::WHITE); // x2 y1
  board[2] = new Bishop(FigureColor::WHITE); // x3 y1
  board[3] = new Queen(FigureColor::WHITE);
  board[4] = new King(FigureColor::WHITE);
  board[5] = new Bishop(FigureColor::WHITE);
  board[6] = new Knight(FigureColor::WHITE);
  board[7] = new Rook(FigureColor::WHITE);
	int i = 8;
	for (; i<16 ;i++)
    board[i] = new Pawn(FigureColor::WHITE); //x2 y1//1+(2-1)*8-1 = 8
	
	for (; i<48 ;i++)
    board[i] = nullptr;
	
	for (; i<56 ;i++)
    board[i] = new Pawn(FigureColor::BLACK);

  board[56] = new Rook(FigureColor::BLACK);//x8 y1//1+(8-1)*8-1 = 56
  board[57] = new Knight(FigureColor::BLACK);
  board[58] = new Bishop(FigureColor::BLACK);
  board[59] = new Queen(FigureColor::BLACK);
  board[60] = new King(FigureColor::BLACK);
  board[61] = new Bishop(FigureColor::BLACK);
  board[62] = new Knight(FigureColor::BLACK);
  board[63] = new Rook(FigureColor::BLACK);
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

	return true;
}

Figure* Board::getIsOccupied(int newCoordinateX, int newCoordinateY) {
	Figure* temp = board[newCoordinateX - 1 + (newCoordinateY - 1)*8];
	//we need temp.color to know if its our color
	return temp;
}