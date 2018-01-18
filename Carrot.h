#pragma once

#include "Plant.h"

class Carrot : public Plant<Settings::CarrotLifeSpan> {
  public:
	Carrot() : Plant(Settings::Carrot) {}
	virtual ~Carrot() = default;
};
