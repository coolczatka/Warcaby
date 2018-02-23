#include "stdafx.h"
#include "Gui.h"

Gui::Gui()
{
	window = new sf::RenderWindow{ sf::VideoMode{ BOK_KW * 8,BOK_KW * 8 },"Warcaby" };
	window->setFramerateLimit(60);
	clicked.setX(-1);
	mcwr = 0;
}

void Gui::run(Board & b, Logic & log)
{
	this->display(b,log);
	this->eventListener(b, clicked, log);
	window->display();
}

void Gui::showWinner(Board & b, Logic & log)
{
	this->display(b,log);
	sf::RectangleShape q(sf::Vector2f(8*BOK_KW,8*BOK_KW));
	q.setFillColor(sf::Color(0, 0, 0, 200));
	q.setPosition(0, 0);
	window->draw(q);
	sf::Vector2i f =window->getPosition();
	sf::Text s;
	int x = log.checkWinningCondition(b);
	if (x == 2)
	{
		s.setString("Wygral bialy");
	}
	else if (x == 1)
	{
		s.setString("Wygral czarny");
	}
	else if (x == 3)
	{
		s.setString("Pat");
	}
	s.setFillColor(sf::Color::Blue);
	sf::Font font;
	if (font.loadFromFile("Risque.ttf"))
		s.setFont(font);
	s.setPosition(70, 200);
	s.setCharacterSize(70);
	window->draw(s);
	sf::Event e;
	window->pollEvent(e);
	if (e.type == sf::Event::Closed)
	{
		window->close();
	}
	window->display();
}

void Gui::display(Board &b, Logic &log)
{
	window->clear(sf::Color::Black);
	sf::CircleShape bgndKing(0.5*BOK_KW - 5);
	sf::CircleShape currentContent(0.5*BOK_KW - 10);
	sf::RectangleShape currentSquare(sf::Vector2f(BOK_KW, BOK_KW));
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Field* f = b.getFieldWithXY(i, j);
			currentSquare.setPosition((float)i*BOK_KW, (float)j * BOK_KW);
			currentContent.setPosition(i*BOK_KW + 10, j*BOK_KW + 10);
			if (f->getFieldColor() == WHITE)
			{
				currentSquare.setFillColor(sf::Color{ 255, 230, 170 });
				window->draw(currentSquare);
			}
			else
			{
				currentSquare.setFillColor(sf::Color{ 210, 140, 70 });
				window->draw(currentSquare);
			}
			if (*f == clicked)
			{
				continue;
			}
			switch (f->getCon())
			{
			case WHITE_CHECKER:
				currentContent.setFillColor(sf::Color(255, 255, 255));
				window->draw(currentContent);
				break;
			case WHITE_KING:
				bgndKing.setPosition(i*BOK_KW + 5, j*BOK_KW + 5);
				bgndKing.setFillColor(sf::Color(255, 0, 0));
				window->draw(bgndKing);
				currentContent.setFillColor(sf::Color(255, 255, 255));
				window->draw(currentContent);
				break;
			case DARK_CHECKER:
				currentContent.setFillColor(sf::Color(0, 0, 0));
				window->draw(currentContent);
				break;
			case DARK_KING:
				bgndKing.setPosition(i*BOK_KW + 5, j*BOK_KW + 5);
				bgndKing.setFillColor(sf::Color(255, 0, 0));
				window->draw(bgndKing);
				currentContent.setFillColor(sf::Color(0, 0, 0));
				window->draw(currentContent);

			}
		}
	}
}


bool Gui::eventListener(Board &b, Field &f, Logic &log)
{
	sf::Event e;
	sf::RectangleShape currentSquare(sf::Vector2f(BOK_KW, BOK_KW));
	sf::CircleShape pion(0.5*BOK_KW - 10);
	sf::CircleShape krol(0.5*BOK_KW - 5);
	window->pollEvent(e);
	if (e.type == sf::Event::Closed)
	{
		window->close();
		return true;
	}
	else if (log.getWm() != b.getColorHuman())
	{
		int maxp = 1;
		bool bylo = 0;
		std::vector<Field> t;
		std::vector<Move> ewq;
		Move w;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{

				if ((i + j) % 2 == 1 && (b.getCopyField(i, j).getCon() == DARK_CHECKER || b.getCopyField(i, j).getCon() == DARK_KING))
				{
					t = log.isCapturePossible(b, b.getCopyField(i, j), log.getWm());
					w.from = b.getCopyField(i, j);
					w.prio = 4;
					for (int k = 0; k < t.size(); k++)
					{
						w.to = t[k];
						for (int l = 0; l < 8; l++)
						{
							for (int m = 0; m < 8; m++)
							{
								Field pole = b.getCopyField(l, m);
								if (pole.getCon() == WHITE_CHECKER || pole.getCon() == WHITE_KING)
								{
									auto hp = b;
									hp.setSthOnField(b.getCopyField(i, j).getCon(), t[k].getX(), t[k].getY());
									hp.setSthOnField(EMPTY, b.getCopyField(i, j).getX(), b.getCopyField(i, j).getY());
									auto mb = log.isCapturePossible(hp, pole, WHITE);
									int znx,zny;
									if (l < t[k].getX())
										znx = 1;
									else
										znx = -1;
									if (m < t[k].getY())
										zny = 1;
									else
										zny = -1;
									auto pp = b.getCopyField(t[k].getX() + znx, t[k].getY() + zny);
									for (int n = 0; n < mb.size(); n++)
									{
										if (mb[n] == pp)
											w.prio=3;
									}
								}
							}
						}
						ewq.push_back(w);
						if (!bylo)
							bylo = 1;
					}
					w.prio = 2;
					t = log.possibleMoves(b, b.getCopyField(i, j));
					for (int k = 0; k < t.size(); k++)
					{
						w.to = t[k];
						for (int l = 0; l < 8; l++)
						{
							for (int m = 0; m < 8; m++)
							{
								Field pole = b.getCopyField(l, m);
								if (pole.getCon() == WHITE_CHECKER || pole.getCon() == WHITE_KING)
								{
									auto hp = b;
									hp.setSthOnField(b.getCopyField(i, j).getCon(), t[k].getX(), t[k].getY());
									hp.setSthOnField(EMPTY, b.getCopyField(i, j).getX(), b.getCopyField(i, j).getY());
									auto mb = log.isCapturePossible(hp, pole, WHITE);
									int znx, zny;
									if (l < t[k].getX())
										znx = 1;
									else
										znx = -1;
									if (m < t[k].getY())
										zny = 1;
									else
										zny = -1;
									auto pp = b.getCopyField(t[k].getX() + znx, t[k].getY() + zny);
									for (int n = 0; n < mb.size(); n++)
									{
										if (mb[n] == pp)
											w.prio = 1;
									}
								}
							}
						}
						ewq.push_back(w);
						if (!bylo)
							bylo = 1;
					}

				}
			}
		}


		srand(time(NULL));
		if (ewq.size() == 0)
			return 0;
		maxp = 0;
		for (int i = 0; i < ewq.size(); i++)
		{
			if (ewq[i].prio > maxp)
				maxp = ewq[i].prio;
		}
		std::vector<Move> qwe;
		for (int i = 0; i < ewq.size(); i++)
		{
			if (ewq[i].prio == maxp)
				qwe.push_back(ewq[i]);
		}
		int kt = rand() % qwe.size();
		if (qwe[kt].prio == 4|| qwe[kt].prio == 3)
		{
			Content zaw = b.getFieldWithXY(qwe[kt].from.getX(), qwe[kt].from.getY())->getCon();
			b.getFieldWithXY(qwe[kt].from.getX(), qwe[kt].from.getY())->setCon(EMPTY);
			int start,stop;
			start = clock();
			stop = clock();
			sf::CircleShape c(0.5*BOK_KW - 10);
			sf::CircleShape k(0.5*BOK_KW - 5);
			c.setFillColor(sf::Color(0, 0, 0));
			k.setFillColor(sf::Color(255, 0, 0));
			int znx, zny, ile;
			ile = qwe[kt].to.getX() - qwe[kt].from.getX();
			znx = (qwe[kt].from.getX() < qwe[kt].to.getX()) ? 1 : -1;
			zny = (qwe[kt].from.getY() < qwe[kt].to.getY()) ? 1 : -1;
			for (int i = 0; i < BOK_KW*abs(ile); i+=4)
			{
				c.setPosition(qwe[kt].from.getX()*BOK_KW +znx*i + 5, qwe[kt].from.getY()*BOK_KW +zny*i + 5);
				k.setPosition(qwe[kt].from.getX()*BOK_KW + znx*i, qwe[kt].from.getY()*BOK_KW + zny*i);
				this->display(b, log);
				(qwe[kt].from.getCon() == DARK_KING)?window->draw(k):znx=1*znx;
				window->draw(c);
				window->display();
			}
			
			b.getFieldWithXY(qwe[kt].from.getX(), qwe[kt].from.getY())->setCon(zaw);
			log.capture(b, qwe[kt].from, qwe[kt].to);
			std::cout << ((b.getColorHuman() == WHITE) ? "C:" : "B:") << "(" << qwe[kt].from.getX() << "," << qwe[kt].from.getY() << ") -> (" << qwe[kt].to.getX() << "," << qwe[kt].to.getY() << ")x\n";

		}
		else
		{
			Content zaw = b.getFieldWithXY(qwe[kt].from.getX(), qwe[kt].from.getY())->getCon();
			b.getFieldWithXY(qwe[kt].from.getX(), qwe[kt].from.getY())->setCon(EMPTY);
			sf::CircleShape c(0.5*BOK_KW - 10);
			sf::CircleShape k(0.5*BOK_KW - 5);
			c.setFillColor(sf::Color(0, 0, 0));
			k.setFillColor(sf::Color(255, 0, 0));
			int znx, zny, ile;
			ile = qwe[kt].to.getX() - qwe[kt].from.getX();
			znx = (qwe[kt].from.getX() < qwe[kt].to.getX()) ? 1 : -1;
			zny = (qwe[kt].from.getY() < qwe[kt].to.getY()) ? 1 : -1;
			for (int i = 0; i < BOK_KW*abs(ile); i += 4)
			{
				c.setPosition(qwe[kt].from.getX()*BOK_KW + znx*i + 5, qwe[kt].from.getY()*BOK_KW + zny* i + 5);
				k.setPosition(qwe[kt].from.getX()*BOK_KW + znx*i, qwe[kt].from.getY()*BOK_KW + zny*i);
				this->display(b, log);
				(qwe[kt].from.getCon() == DARK_KING) ? window->draw(k) : znx = 1 * znx;
				window->draw(c);
				window->display();
			}
			b.getFieldWithXY(qwe[kt].from.getX(), qwe[kt].from.getY())->setCon(DARK_CHECKER);
			log.move(b, qwe[kt].from, qwe[kt].to);
			std::cout << ((b.getColorHuman() == WHITE) ? "C:" : "B:") << "(" << qwe[kt].from.getX() << "," << qwe[kt].from.getY() << ") -> (" << qwe[kt].to.getX() << "," << qwe[kt].to.getY() << ")\n";
		}
		if (qwe[kt].to.getY() == 7)
		{
			log.changeInKing(b, qwe[kt].to.getX(), qwe[kt].to.getY());
			std::cout << "^K\n";
		}
	}
	else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && !mcwr)
	{
		f = getClickedField(e.mouseButton.x, e.mouseButton.y, b);
		if (f.getCon() == EMPTY|| f.getCon() == DARK_CHECKER|| f.getCon() == DARK_KING)
			return false;
		mcwr = true;
		currentSquare.setPosition(f.getX()*BOK_KW, f.getY() * BOK_KW);
		currentSquare.setFillColor(sf::Color{ 210, 140, 70 });
		std::cout << ((b.getColorHuman() == WHITE) ? "B:" : "C:") << "(" << f.getX() << "," << f.getY() << ")";
		window->draw(currentSquare);
	}
	else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right && mcwr)
	{
		mcwr = false;
		f.setX(-1);
		std::cout << "\nAnulowano" << std::endl;
		putGr(f);
	}
	else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && mcwr && log.getWm() == b.getColorHuman() && (f.getCon()==WHITE_CHECKER||f.getCon()==WHITE_KING))
	{
		int maxp = 1;
		bool bylo = 0;
		std::vector<Field> t;
		std::vector<Move> ewq;
		Move w;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{

				if ((i + j) % 2 == 1 && (b.getCopyField(i, j).getCon() == WHITE_CHECKER || b.getCopyField(i, j).getCon() == WHITE_KING))
				{
					t = log.isCapturePossible(b, b.getCopyField(i, j), log.getWm());
					w.from = b.getCopyField(i, j);
					w.prio = 2;
					for (int k = 0; k < t.size(); k++)
					{
						w.to = t[k];
						ewq.push_back(w);
						if (!bylo)
							bylo = 1;
					}
				}
			}
		}
		mcwr = 0;
		bool tr = false;
		Field q = getClickedField(e.mouseButton.x, e.mouseButton.y, b);
		if (ewq.size() > 0)
		{
			for (int i = 0; i < ewq.size(); i++)
			{
				if (ewq[i].from == f && ewq[i].to == q)
				{
					log.capture(b, f, q);
					std::cout << "-> (" << q.getX() << "," << q.getY() << ")x\n";
					if (log.changeInKing(b, q.getX(), q.getY()))
						std::cout << "^K\n";
					return 1;
				}
			}
		}
		else
			tr = log.move(b, clicked, q);
		if (tr)
		{
			std::cout << " -> (" << q.getX() << "," << q.getY() << ")\n";
			if (log.changeInKing(b, q.getX(), q.getY()))
				std::cout << "^K\n";
			f.setX(-1);
			return true;
		}
		if (tr)
		{
			std::cout << " -> (" << q.getX() << "," << q.getY() << ")x" << "\n";
			if (log.changeInKing(b, q.getX(), q.getY()))
				std::cout << "^K\n";
			f.setX(-1);
			return true;
		}
		else
		{
			std::cout << "Bledny ruch " << std::endl;
			putGr(f);
			f.setX(-1);
			return false;
		}
	}
	else if (mcwr)
	{
		sf::Vector2f mysz(sf::Mouse::getPosition(*window));
		if (f.getCon() == WHITE_KING)
		{
			krol.setFillColor(sf::Color(255, 0, 0, 255));
			krol.setPosition(mysz.x-(0.5*BOK_KW)+7, mysz.y-(0.5*BOK_KW)+7);
			window->draw(krol);
		}
		pion.setFillColor(sf::Color(255, 255, 255, 255));
		pion.setPosition(mysz.x-int(BOK_KW/3), mysz.y - int(BOK_KW / 3));
		window->draw(pion);
	}
	return false;
}

void Gui::putGr(Field f)
{
	sf::CircleShape bgndKing(0.5*BOK_KW - 5);
	sf::CircleShape currentContent(0.5*BOK_KW - 10);
	switch (f.getCon())
	{
	case WHITE_CHECKER:
		currentContent.setFillColor(sf::Color(255, 255, 255));
		window->draw(currentContent);
		break;
	case WHITE_KING:
		bgndKing.setPosition(f.getX()*BOK_KW + 5, f.getY()*BOK_KW + 5);
		bgndKing.setFillColor(sf::Color(255, 0, 0));
		window->draw(bgndKing);
		currentContent.setFillColor(sf::Color(255, 255, 255));
		window->draw(currentContent);
		break;
	case DARK_CHECKER:
		currentContent.setFillColor(sf::Color(0, 0, 0));
		window->draw(currentContent);
		break;
	case DARK_KING:
		bgndKing.setPosition(f.getX()*BOK_KW + 5, f.getY()*BOK_KW + 5);
		bgndKing.setFillColor(sf::Color(255, 0, 0));
		window->draw(bgndKing);
		currentContent.setFillColor(sf::Color(0, 0, 0));
		window->draw(currentContent);
	}
}

Field Gui::getClickedField(short x, short y, Board b)
{
	unsigned short p, q;
	p = static_cast<int>(x / BOK_KW);
	q = static_cast<int>(y / BOK_KW);
	return b.getCopyField(p, q);
}

Gui::~Gui()
{
	delete window;
}
