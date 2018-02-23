#include "stdafx.h"
#include "Logic.h"

bool Logic::move(Board &b, Field f, Field t)
{
	if (f.getCon() == WHITE_CHECKER)
	{
		if (((f.getX() == t.getX() - 1 || f.getX() == t.getX() + 1) && f.getY() == t.getY() + 1) && !isSthOnField(b, t.getX(), t.getY()))
		{
			Field*fa;
			wpm = wm;
			wm = DARK;
			fa = b.getFieldWithXY(f.getX(), f.getY());
			fa->setCon(EMPTY);
			fa = b.getFieldWithXY(t.getX(), t.getY());
			fa->setCon(WHITE_CHECKER);
			return true;
		}
	}
	else if (f.getCon() == DARK_CHECKER)
	{
		if (((f.getX() == t.getX() - 1 || f.getX() == t.getX() + 1) && f.getY() == t.getY() - 1) && !isSthOnField(b, t.getX(), t.getY()))
		{
			wpm = wm;
			wm = WHITE;
			b.getFieldWithXY(f.getX(), f.getY())->setCon(EMPTY);
			b.getFieldWithXY(t.getX(), t.getY())->setCon(DARK_CHECKER);
			return true;
		}
	}
	else if (f.getCon() == WHITE_KING || f.getCon() == DARK_KING)
	{
		short dirX = 1, dirY = 1;//0 - lewo/dol
		bool tsno = 1;
		if (f.getX() > t.getX())
			dirX = -1;
		if (f.getY() > t.getY())
			dirY = -1;
		for (int i = 1; i <= abs(f.getX() - t.getX()); i++)
		{
			if (isSthOnField(b, f.getX() + i*dirX, f.getY() + i*dirY))
				tsno = 0;
		}
		if (!isSthOnField(b, t.getX(), t.getY()) && tsno)
		{
			if (wm == WHITE)
			{
				wpm = wm;
				wm = DARK;
				b.getFieldWithXY(f.getX(), f.getY())->setCon(EMPTY);
				b.getFieldWithXY(t.getX(), t.getY())->setCon(WHITE_KING);
			}
			else
			{
				wpm = wm;
				wm = WHITE;
				b.getFieldWithXY(f.getX(), f.getY())->setCon(EMPTY);
				b.getFieldWithXY(t.getX(), t.getY())->setCon(DARK_KING);
			}
			return true;
		}

		else
			return false;
	}
	return false;
}

short Logic::checkWinningCondition(Board b)
{
	if (b.getHmw() == 0)
		return 1;
	else if (b.getHmd() == 0)
		return 2;
	bool bylo = 0;
	std::vector<Field> t;
	std::vector<Move> cz;
	std::vector<Move> bi;
	Move w;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			if ((i + j) % 2 == 1 && (b.getCopyField(i, j).getCon() == DARK_CHECKER || b.getCopyField(i, j).getCon() == DARK_KING))
			{
				t = isCapturePossible(b, b.getCopyField(i, j), getWm());
				w.from = b.getCopyField(i, j);
				w.prio = 2;
				for (int k = 0; k < t.size(); k++)
				{
					w.to = t[k];
					cz.push_back(w);
					if (!bylo)
						bylo = 1;
				}
				w.prio = 1;
				t = possibleMoves(b, b.getCopyField(i, j));
				for (int k = 0; k < t.size(); k++)
				{
					w.to = t[k];
					cz.push_back(w);
				}

			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			if ((i + j) % 2 == 1 && (b.getCopyField(i, j).getCon() == WHITE_CHECKER || b.getCopyField(i, j).getCon() == WHITE_KING))
			{
				t = isCapturePossible(b, b.getCopyField(i, j), getWm());
				w.from = b.getCopyField(i, j);
				w.prio = 2;
				for (int k = 0; k < t.size(); k++)
				{
					w.to = t[k];
					bi.push_back(w);
					if (!bylo)
						bylo = 1;
				}
				w.prio = 1;
				t = possibleMoves(b, b.getCopyField(i, j));
				for (int k = 0; k < t.size(); k++)
				{
					w.to = t[k];
					bi.push_back(w);
				}

			}
		}
	}
	if ((cz.size() == 0 && wm==DARK) || (bi.size() == 0)&& wm==WHITE)
		return 3;
	else
		return 0;
}

bool Logic::changeInKing(Board &b, short qx, short qy)
{
	Field* q = b.getFieldWithXY(qx, qy);
	if (q->getY() == 0 && q->getCon() == WHITE_CHECKER)
	{
		q->setCon(WHITE_KING);
		wm = DARK;
		return true;
	}
	else if (q->getY() == 7 && q->getCon() == DARK_CHECKER)
	{
		q->setCon(DARK_KING);
		wm = WHITE;
		return true;
	}
	return false;
}

Color Logic::getWm()
{
	return wm;
}

Color Logic::getWpm()
{
	return wpm;
}

bool Logic::isSthOnField(Board b, Content c, short x, short y)
{
	if ((*(b.getFieldWithXY(x, y))).getCon() == c)  // je¿eli pole o wspolrzêdnych x y ma zawartoœæ  c
		return true;
	return false;
}

bool Logic::isSthOnField(Board b, short x, short y)
{
	if ((*(b.getFieldWithXY(x, y))).getCon() != EMPTY)  // je¿eli pole o wspolrzêdnych x y ma wartosc EMPTY
		return true;
	return false;
}

std::vector<Field> Logic::isCapturePossible(Board b, Field f, Color c)
{
	std::vector<Field> vec;
	if (c == WHITE)
	{
		if (f.getCon() == WHITE_CHECKER)
		{
			bool pg, lg, pd, ld;
			pg = (f.getY() > 1) && (f.getX() < 6) && (isSthOnField(b, DARK_CHECKER, f.getX() + 1, f.getY() - 1) || isSthOnField(b, DARK_KING, f.getX() + 1, f.getY() - 1)) && !isSthOnField(b, f.getX() + 2, f.getY() - 2);
			lg = (f.getY() > 1) && (f.getX() > 1) && (isSthOnField(b, DARK_CHECKER, f.getX() - 1, f.getY() - 1) || isSthOnField(b, DARK_KING, f.getX() - 1, f.getY() - 1)) && !isSthOnField(b, f.getX() - 2, f.getY() - 2);
			pd = (f.getY() < 6) && (f.getX() < 6) && (isSthOnField(b, DARK_CHECKER, f.getX() + 1, f.getY() + 1) || isSthOnField(b, DARK_KING, f.getX() + 1, f.getY() + 1)) && !isSthOnField(b, f.getX() + 2, f.getY() + 2);
			ld = (f.getY() < 6) && (f.getX() > 1) && (isSthOnField(b, DARK_CHECKER, f.getX() - 1, f.getY() + 1) || isSthOnField(b, DARK_KING, f.getX() - 1, f.getY() + 1)) && !isSthOnField(b, f.getX() - 2, f.getY() + 2);
			if (pg)
				vec.push_back(b.getCopyField(f.getX() + 2, f.getY() - 2));
			if (lg)
				vec.push_back(b.getCopyField(f.getX() - 2, f.getY() - 2));
			if (pd)
				vec.push_back(b.getCopyField(f.getX() + 2, f.getY() + 2));
			if (ld)
				vec.push_back(b.getCopyField(f.getX() - 2, f.getY() + 2));
		}
		else if (f.getCon() == WHITE_KING)
		{
			//pd
			int dirX = 1, dirY = 1;
			int x = f.getX()+dirX,y = f.getY()+dirY;
			while (x < 7 && y < 7)
			{
				if ((isSthOnField(b, DARK_CHECKER, x, y) || isSthOnField(b, DARK_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
			dirX = -1; //ld
			x = f.getX() + dirX, y = f.getY() + dirY;
			while (x > 0 && y < 7)
			{
				if ((isSthOnField(b, DARK_CHECKER, x, y) || isSthOnField(b, DARK_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
			dirY = -1; //lg
			x = f.getX() + dirX, y = f.getY() + dirY;
			while (x > 0 && y > 0)
			{
				if ((isSthOnField(b, DARK_CHECKER, x, y) || isSthOnField(b, DARK_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
			dirX = 1; //pg
			x = f.getX() + dirX, y = f.getY() + dirY;
			while (x < 7 && y > 0)
			{
				if ((isSthOnField(b, DARK_CHECKER, x, y) || isSthOnField(b, DARK_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}

		}
	}
	else
	{
		if (f.getCon() == DARK_CHECKER)
		{
			bool pg, lg, pd, ld;
			pg = (f.getY() > 1) && (f.getX() < 6) && (isSthOnField(b, WHITE_CHECKER, f.getX() + 1, f.getY() - 1) || isSthOnField(b, WHITE_KING, f.getX() + 1, f.getY() - 1)) && !isSthOnField(b, f.getX() + 2, f.getY() - 2);
			lg = (f.getY() > 1) && (f.getX() > 1) && (isSthOnField(b, WHITE_CHECKER, f.getX() - 1, f.getY() - 1) || isSthOnField(b, WHITE_KING, f.getX() - 1, f.getY() - 1)) && !isSthOnField(b, f.getX() - 2, f.getY() - 2);
			pd = (f.getY() < 6) && (f.getX() < 6) && (isSthOnField(b, WHITE_CHECKER, f.getX() + 1, f.getY() + 1) || isSthOnField(b, WHITE_KING, f.getX() + 1, f.getY() + 1)) && !isSthOnField(b, f.getX() + 2, f.getY() + 2);
			ld = (f.getY() < 6) && (f.getX() > 1) && (isSthOnField(b, WHITE_CHECKER, f.getX() - 1, f.getY() + 1) || isSthOnField(b, WHITE_KING, f.getX() - 1, f.getY() + 1)) && !isSthOnField(b, f.getX() - 2, f.getY() + 2);
			if (pg)
				vec.push_back(b.getCopyField(f.getX() + 2, f.getY() - 2));
			if (lg)
				vec.push_back(b.getCopyField(f.getX() - 2, f.getY() - 2));
			if (pd)
				vec.push_back(b.getCopyField(f.getX() + 2, f.getY() + 2));
			if (ld)
				vec.push_back(b.getCopyField(f.getX() - 2, f.getY() + 2));
		}
		else if (f.getCon() == DARK_KING)
		{
			//pd
			int dirX = 1, dirY = 1;
			int x = f.getX() + dirX, y = f.getY() + dirY;
			while (x < 7 && y < 7)
			{
				if ((isSthOnField(b, WHITE_CHECKER, x, y) || isSthOnField(b, WHITE_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
			dirX = -1; //ld
			x = f.getX() + dirX, y = f.getY() + dirY;
			while (x > 0 && y < 7)
			{
				if ((isSthOnField(b, WHITE_CHECKER, x, y) || isSthOnField(b, WHITE_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
			dirY = -1; //lg
			x = f.getX() + dirX, y = f.getY() + dirY;
			while (x > 0 && y > 0)
			{
				if ((isSthOnField(b, WHITE_CHECKER, x, y) || isSthOnField(b, WHITE_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
			dirX = 1; //pg
			x = f.getX() + dirX, y = f.getY() + dirY;
			while (x < 7 && y > 0)
			{
				if ((isSthOnField(b, WHITE_CHECKER, x, y) || isSthOnField(b, WHITE_KING, x, y)) && !isSthOnField(b, x + dirX, y + dirY))
				{
					vec.push_back(b.getCopyField(x + dirX, y + dirY));
					break;
				}
				if (isSthOnField(b, x, y))
					break;
				x += dirX;
				y += dirY;
			}
		}
	}
	return vec;
}

std::vector<Field> Logic::possibleMoves(Board b, Field f)
{
	std::vector<Field> v;
	if (f.getCon() == WHITE_CHECKER)
	{
		if (!isSthOnField(b, f.getX() - 1, f.getY() - 1) && f.getX()>0 && f.getY()>0)
			v.push_back(b.getCopyField(f.getX() - 1, f.getY() - 1));
		if (!isSthOnField(b, f.getX() + 1, f.getY() - 1) && f.getX()<7 && f.getY()>0)
			v.push_back(b.getCopyField(f.getX() + 1, f.getY() - 1));
	}
	else if (f.getCon() == DARK_CHECKER)
	{
		
		if (!isSthOnField(b, f.getX() - 1, f.getY() + 1) && f.getX()>0 && f.getY()<7)
			v.push_back(b.getCopyField(f.getX() - 1, f.getY() + 1));
		if (!isSthOnField(b, f.getX() + 1, f.getY() + 1) && f.getX()<7 && f.getY()<7)
			v.push_back(b.getCopyField(f.getX() + 1, f.getY() + 1));
	}
	else if (f.getCon() == WHITE_KING || f.getCon() == DARK_KING)
	{
		//pd
		int dirX = 1, dirY = 1;
		int x = f.getX() + dirX, y = f.getY() + dirY;
		while (x < 8 && y < 8 && f.getX()<7 && f.getY()<7)
		{
			if (!isSthOnField(b, x, y))
			{
				v.push_back(b.getCopyField(x, y));
			}
			if (isSthOnField(b, x, y))
				break;
			x += dirX;
			y += dirY;
		}
		dirX = -1; //ld
		x = f.getX() + dirX, y = f.getY() + dirY;
		while (x > 0 && y < 7 && f.getX()>0 && f.getY()<7)
		{
			if (!isSthOnField(b, x, y))
			{
				v.push_back(b.getCopyField(x, y));
			}
			if (isSthOnField(b, x, y))
				break;
			x += dirX;
			y += dirY;
		}
		dirY = -1; //lg
		x = f.getX() + dirX, y = f.getY() + dirY;
		while (x > 0 && y > 0 && f.getX()>0 && f.getY()>0)
		{
			if (!isSthOnField(b, x, y))
			{
				v.push_back(b.getCopyField(x, y));
			}
			if (isSthOnField(b, x, y))
				break;
			x += dirX;
			y += dirY;
		}
		dirX = 1; //pg
		x = f.getX() + dirX, y = f.getY() + dirY;
		while (x < 7 && y > 0 && f.getX()<7 && f.getY()>0)
		{
			if (!isSthOnField(b, x, y))
			{
				v.push_back(b.getCopyField(x, y));
			}
			if (isSthOnField(b, x, y))
				break;
			x += dirX;
			y += dirY;
		}
	}
	return v;
}


bool Logic::capture(Board &b, Field f, Field t)
{
	std::vector<Field>vec = isCapturePossible(b,f,wm);
	int dirX = 1, dirY = 1;	
	bool res = 0, nt = 1;
	if (f.getX() > t.getX())
		dirX = -1;
	if (f.getY() > t.getY())
		dirY = -1;
	for (int i=0;i<vec.size();i++)
	{
		if (vec[i] == t)
		{
			b.setSthOnField(EMPTY, f.getX(), f.getY());
			b.setSthOnField(EMPTY, t.getX() - dirX, t.getY() - dirY);
			b.setSthOnField(f.getCon(), t.getX(), t.getY());
			wm == WHITE ? b.decrDark() : b.decrWhite();
			res = true;
		}
	}
	if (res)
	{
		t = b.getCopyField(t.getX(), t.getY());
		vec = isCapturePossible(b, t, wm);
		if (vec.size() == 0)
		{
			if (wm == WHITE)
			{
				wpm = wm;
				wm = DARK;
			}
			else
			{
				wpm = wm;
				wm = WHITE;
			}
		}
	}
	return res;
}
Logic::~Logic()
{
}
