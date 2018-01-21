#pragma once

#include "ICreature.h"

class IPlant : public ICreature {
  public:
	virtual bool process() override = 0;
};