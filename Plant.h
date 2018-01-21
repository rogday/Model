#pragma once

#include "IPlant.h"
#include "Settings.h"

template <Settings::Types type, int LifeSpan> class Plant : public IPlant {
  public:
	Plant() = default;
	virtual bool process() override {
		pass ^= true;
		return ++age <= LifeSpan;
	}
};

using Carrot = Plant<Settings::Carrot, Settings::CarrotLifeSpan>;