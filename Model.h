#pragma once

#include "Settings.h"

class Model {
  private:
	Map field; // Sex and species as dimentions

  public:
	Model(int n, int m);

	Cell &at(int x, int y) { return field[x][y]; }
	void kill(Settings::Types type, int x, int y);
	void add(Settings::Types type, int x, int y);
	Cell::iterator get(Settings::Types type, int x, int y);

	void processField();
};
