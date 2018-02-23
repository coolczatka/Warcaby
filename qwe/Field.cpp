#include "stdafx.h"
#include "Field.h"

short Field::getX()
{
	return x;
}

short Field::getY()
{
	return y;
}

Color Field::getFieldColor()
{
	return fieldColor;
}

Content Field::getCon()
{
	return con;
}

void Field::setX(short xx)
{
	x = xx;
}

void Field::setY(short yy)
{
	y = yy;
}

void Field::setFieldColor(Color f)
{
	fieldColor = f;
}

void Field::setCon(Content c)
{
	con = c;
}

bool Field::operator==(Field f)
{
	if (this->getX() == f.getX() && this->getY() == f.getY())
		return true;
	else
		return false;
}
