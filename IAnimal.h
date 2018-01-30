#pragma once

#include "ICreature.h"

class IAnimal : public ICreature {
  public:
	IAnimal(bool state) : ICreature(state){};
	virtual bool process() override = 0;
	virtual int move() = 0;

	virtual bool hasMoves() = 0;

	virtual bool getSex() const = 0;
	virtual int getDelay() const = 0;
};