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
	FigureColor getColor();
	void setColor();
	virtual int **move(int coordinateX, int coordinateY);
	bool getIsUnderCheck();
	bool getIsUnderPin();

private:

	FigureColor m_Color;
	bool m_IsUnderCheck;
	bool m_IsUnderPin;
};