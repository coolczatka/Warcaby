#pragma once
#include "Field.h"
class Board
{
	Color colorHuman;
	Field fields[64];
	short hmw;
	short hmd;
public:
	Board(Color colorHuman);
	Color getColorHuman();
	Field* getFieldWithXY(short x, short y);
	void setSthOnField(Content c, short x, short y);
	Field getCopyField(short x, short y);
	void decrWhite();
	void decrDark();
	short getHmw();
	short getHmd();
	~Board();
};

