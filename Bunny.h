#pragma once

#include "Animal.h"
#include "Settings.h"

class Bunny : public Animal<Settings::BunnyLifeSpan, Settings::CarrotNuVal,
							Settings::BunnySpeed, Settings::BunnyFOV,
							Settings::Types::Carrot, Settings::Types::Fox> {
  public:
	Bunny(int i, int k) : Animal(i, k, Settings::Bunny) {}
	virtual ~Bunny() = default;
};
