#pragma once

#include "ICreature.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>

class IAnimal;

class Model {
  private:
	std::vector<ICreature *> field;
	bool pass;
	sf::RenderWindow window;
	std::vector<sf::RectangleShape> rects;

	/*statistics stuff */
	std::vector<int> born;
	std::vector<int> died;

  private:
	ICreature *&at(int x, int y, int r) {
		return field[Settings::N * Settings::M * r + y * Settings::N + x];
	};
	sf::RectangleShape &rat(int x, int y) {
		return rects[y * Settings::N + x];
	};
	void move(int x, int y, int r, int way);

  public:
	Model();
	// kill and add
	void kill(int x, int y, int r);
	void add(int x, int y, int r);
	bool empty(int x, int y, int r) { return !at(x, y, r)->alive(); }

	bool pinkTicket(int x, int y, int r, IAnimal *two);

	void processField();
	void start();
	void prettyPrint();

	// do not forget to clean up memory
	~Model();
};
