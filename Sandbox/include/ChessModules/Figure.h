#pragma once
#define MAX_COORD 8
#define MIN_COORD 1 //vremenno sa tuk defainite

enum FigureColor {
	NULL_COLOR,
	WHITE,
	BLACK
};

class Figure {
public:

	Figure();
	Figure(FigureColor color);

	FigureColor getColor() { return m_Color; }
	void setColor(FigureColor color) { m_Color = color; }// Why this???
	virtual int** move(int coordinateX, int coordinateY) = 0;

private:
	FigureColor m_Color;
};