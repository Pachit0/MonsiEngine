#include "ChessStdafx.h"

Figure::Figure(bool color) {
		this->color = color;
}



bool Figure::move(char *x, char *y) {//to be cleaned
	if (strlen(x) != 2 || strlen(y) != 2) {
		cout << "Invalid command! Too long or too short! Example: x1" << endl;
	  return false;
	}
  if (x[0] != 'x' || y[0] != 'y') {
		cout << "Invalid command! Used different coordinates! You can only use x and y" << endl;
		return false;
	}
	if (x[1] > MAX_COORD || x[1] < MIN_COORD || y[1] > MAX_COORD || y[1] < MIN_COORD) {
    cout << "Invalid command! Too big, too small or negative coordinates used! "
    << "You can only type from " << MIN_COORD
    << " to " << MAX_COORD << endl;
		return false;
	}
	if (getIsUnderCheck() == 1)
		
	
}

