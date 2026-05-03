#pragma once
#define MAX_COORD 8
#define MIN_COORD 1//vremenno sa tuk defainite
enum FigureColor {
	NULL_COLOR,
	WHITE,
	BLACK
};

class Figure {
public:

	Figure();
	Figure(FigureColor color);

	void printStats();
<<<<<<< HEAD
	bool move(char* x, char* y);
=======
	FigureColor getColor();
	void setColor();
	virtual int **move(int coordinateX, int coordinateY);
	bool getIsUnderCheck();
	bool getIsUnderPin();
>>>>>>> d7951cf7e507fe62e81eea20c65aafc7bc2566de

private:
	FigureColor m_Color;
};