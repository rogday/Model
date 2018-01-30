#pragma once

#include "ICreature.h"

class IAnimal : public ICreature {
  public:
	virtual bool process() override = 0;
	virtual int move() = 0;

	virtual bool hasMoves() = 0;

	virtual bool getSex() = 0;
	virtual int getDelay() = 0;

	virtual ~IAnimal() = default;
};