#include "ChessModules/Rook.h"

Rook::Rook(FigureColor Color)
{
	//setColor(Color):Figure(Color)

}

int **Rook::move(int coordinateX, int coordinateY)
{

	int rows = 2;
	if (coordinateX > MIN_COORD && coordinateX < MAX_COORD)
		rows++;
	if (coordinateY > MIN_COORD || coordinateY < MAX_COORD)
		rows++;

	int** possibleMoves = new int* [rows];
	for (int i = 0; i < rows; i++) {
		possibleMoves[i] = new int[MAX_COORD];
	}

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < MAX_COORD; j++)
			possibleMoves[i][j] = -1;
	//min 2 rows ,max 4 rows. Every row is a direction!
	for (int i = coordinateX, j = 0; i < MAX_COORD; i++, j++)
		possibleMoves[j][0] = i - 1 + (coordinateY - 1) * 8;

	for (int i = coordinateY, j = 0; i < MAX_COORD; i++, j++)
		possibleMoves[j][1] = coordinateX - 1 + (i - 1) * 8;

	for (int i = coordinateX, j = 0; i > MIN_COORD; i--, j++)//moje da se naloji naobratno da se zapisvat v masiva, ne ot nai golemi koordinati, za blockirani figuri ot drugi e vajno
		possibleMoves[j][2] = i - 1 + (coordinateY - 1) * 8;

	for (int i = coordinateY, j = 0; i > MIN_COORD; i--, j++)
		possibleMoves[j][3] = coordinateX - 1 + (i - 1) * 8;



	return possibleMoves;
}

