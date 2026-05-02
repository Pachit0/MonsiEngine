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
	bool getIsUnderCheck();
	bool getIsUnderPin();

private:

	FigureColor m_Color;
	bool m_IsUnderCheck;
	bool m_IsUnderPin;
};