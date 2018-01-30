#pragma once

#include "IPlant.h"
#include "Settings.h"

template <Settings::Types type, int LifeSpan> class Plant : public IPlant {
  public:
	Plant(bool state) : IPlant(state){};

	virtual void reset(bool pass) {
		assert(!state);

		state = true;
		age = 0;
		this->pass = pass;
	}

	virtual void swap(ICreature *another) override {
		Plant *ptr = dynamic_cast<Plant *>(another);

		std::swap(state, ptr->state);
		std::swap(age, ptr->age);
		std::swap(pass, ptr->pass);
	}

	virtual bool process() override {
		pass ^= true;
		return ++age <= LifeSpan;
	}
};

using Carrot = Plant<Settings::Carrot, Settings::CarrotLifeSpan>;