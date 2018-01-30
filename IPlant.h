#pragma once

#include "ICreature.h"

class IPlant : public ICreature {
  public:
	IPlant(bool state) : ICreature(state){};
	virtual bool process() override = 0;
};