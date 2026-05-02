#include "ChessModules/ChessCore.h"
#include "ChessModules/Figure.h"

Figure::Figure()
{
	m_Color = NULL_COLOR;
	m_IsUnderPin = false;
	m_IsUnderCheck = false;
}

Figure::Figure(FigureColor color) {
		m_Color = color;
		m_IsUnderPin = false;
		m_IsUnderCheck = false;
}

void Figure::printStats()
{

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
	if (getIsUnderCheck() == 1);
	
	return true;
}

bool Figure::getIsUnderCheck()
{
	return false;
}

bool Figure::getIsUnderPin()
{
	return false;
}

