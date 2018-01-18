#pragma once
#include "Animal.h"
#include "Settings.h"

class Fox : public Animal<Settings::FoxLifeSpan, Settings::BunnyNuVal,
						  Settings::FoxSpeed, Settings::FoxFOV,
						  Settings::Types::Bunny, Settings::Types::None> {
  public:
	Fox(int i, int k) : Animal(i, k, Settings::Fox){};
	virtual ~Fox() = default;
};
