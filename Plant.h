#pragma once

#include "ICreature.h"
#include "Settings.h"

template <int LifeSpan> class Plant : public ICreature {
  public:
	Plant(Settings::Types type) : ICreature(type){};
	virtual bool process() override { return ++age <= LifeSpan; }
	virtual ~Plant() = default;
};
