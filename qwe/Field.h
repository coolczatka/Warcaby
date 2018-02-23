#pragma once
enum Color { WHITE, DARK };
enum Content { WHITE_KING, DARK_KING, WHITE_CHECKER, DARK_CHECKER, EMPTY };
class Field
{
	short x, y;
	Color fieldColor;
	Content con;
public:
	short getX();
	short getY();
	Color getFieldColor();
	Content getCon();
	void setX(short xx);
	void setY(short yy);
	void setFieldColor(Color f);
	void setCon(Content c);
	bool operator==(Field f);

};

