#include "stdafx.h"
#include "Board.h"


Board::Board(Color c)
{
	hmw = 12;
	hmd = 12;
	colorHuman = c;
	if (colorHuman == WHITE)
	{
		for (int i = 0; i < 64; i++)
		{
			fields[i].setX(i % 8);
			fields[i].setY((int)(i / 8));
			if ((fields[i].getY() == 0 || fields[i].getY() == 1 || fields[i].getY() == 2) && (fields[i].getX() + fields[i].getY()) % 2 == 1)
			{
				fields[i].setCon(DARK_CHECKER);
			}
			else if ((fields[i].getY() == 5 || fields[i].getY() == 6 || fields[i].getY() == 7) && (fields[i].getX() + fields[i].getY()) % 2 == 1)
			{
				fields[i].setCon(WHITE_CHECKER);
			}
			else
				fields[i].setCon(EMPTY);
			if ((fields[i].getX()+fields[i].getY()) % 2 == 0)
			{
				fields[i].setFieldColor(WHITE);
				fields[i].setCon(EMPTY);
			}
			else
			{
				fields[i].setFieldColor(DARK);
			}
		}
	}
	else
	{
		for (int i = 63; i >=0; i--)
		{
			fields[i].setX(7-i % 8);
			fields[i].setY((int)(7-i / 8));
			if ((fields[i].getY() == 0 || fields[i].getY() == 1 || fields[i].getY() == 2) && (fields[i].getX() + fields[i].getY()) % 2 == 1)
			{
				fields[i].setCon(DARK_CHECKER);
			}
			else if ((fields[i].getY() == 5 || fields[i].getY() == 6 || fields[i].getY() == 7) && (fields[i].getX() + fields[i].getY()) % 2 == 1)
			{
				fields[i].setCon(WHITE_CHECKER);
			}
			else
				fields[i].setCon(EMPTY);
			if ((fields[i].getX()+fields[i].getY()) % 2 == 0)
			{
				fields[i].setFieldColor(WHITE);
				fields[i].setCon(EMPTY);
			}
			else
			{
				fields[i].setFieldColor(DARK);
			}
			fields[i].setX(fields[i].getX());
			fields[i].setY(7 - fields[i].getY());
		}
	}
}

Color Board::getColorHuman()
{
	return colorHuman;
}

Field * Board::getFieldWithXY(short x, short y)
{
	Field* f = NULL;
	f = &fields[y * 8 + x];
	return f;
}

void Board::setSthOnField(Content c, short x, short y)
{
	Field* xa;
	xa = getFieldWithXY(x, y);
	xa->setCon(c);
}

Field Board::getCopyField(short x, short y)
{
	return fields[y * 8 + x];
}

void Board::decrWhite()
{
	hmw--;
}

void Board::decrDark()
{
	hmd--;
}

short Board::getHmw()
{
	return hmw;
}

short Board::getHmd()
{
	return hmd;
}


Board::~Board()
{
}
