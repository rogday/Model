#pragma once

#include "Settings.h"
#include <SFML/Graphics.hpp>

class IAnimal;

class Model {
  private:
	std::vector<ICreature *> field;
	bool pass;
	sf::RenderWindow window;
	std::vector<sf::RectangleShape> rects;

  private:
	ICreature *&at(int x, int y, int r);
	void move(int x, int y, int r, int way);

  public:
	Model();
	// kill and add
	void kill(int x, int y, int r);
	void add(int x, int y, int r);
	bool empty(int x, int y, int r);

	bool pinkTicket(int x, int y, int r, IAnimal *two);

	void processField();
	void start();
};
