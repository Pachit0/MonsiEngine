#pragma once

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
	bool move(char* x, char* y);

private:
	FigureColor m_Color;
};