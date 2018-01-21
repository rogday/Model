#pragma once

#include "Settings.h"
#include <SFML/Graphics.hpp>

class Model {
  private:
	std::vector<ICreature *> field;
	bool pass;
	sf::RenderWindow window;

  public:
	Model();

	void kill(Settings::Types type, int x, int y);
	void add(Settings::Types type, int x, int y);
	ICreature *&at(int x, int y, int r);

	void processField();
	void start();
};
