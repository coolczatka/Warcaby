#pragma once
#include "Board.h"
#include <cmath>
#include <iostream>
#include <vector>
class Logic
{
	Color wm;
	Color wpm;
	static bool isSthOnField(Board b, Content c, short x, short y);
	static bool isSthOnField(Board b, short x, short y);
public:
	std::vector<Field> isCapturePossible(Board b, Field f, Color c); 
	std::vector<Field> possibleMoves(Board b, Field f);
	Logic(Color m,Color p) :wm(m),wpm(p) {};
	bool capture(Board &b, Field f, Field t);
	bool move(Board &b, Field f, Field t);
	short checkWinningCondition(Board b);
	bool changeInKing(Board &b,short qx, short qy);
	Color getWm();
	Color getWpm();
	~Logic();
};

struct Move
{
	Field from;
	Field to;
	int prio;
};