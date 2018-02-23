#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Board.h"
#include <iostream>
#include "Logic.h"
#include <vector>
#include <ctime>

#define BOK_KW 70 
class Gui
{
	sf::RenderWindow* window;
	bool eventListener(Board &b, Field &f, Logic &log);
	Field clicked;
	void display(Board &b, Logic &log);
	bool mcwr;
	
public:
	void putGr(Field f);
	Gui();
	void run(Board& b, Logic& log);
	void showWinner(Board& b, Logic& log);
	Field getClickedField(short x, short y, Board b);
	~Gui();
};
