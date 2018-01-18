#pragma once

#include "Model.h"
#include "Settings.h"

class ICreature {
  protected:
	Settings::Types type;
	static Model *model;
	int age;

  public:
	ICreature(Settings::Types type) : type(type), age(0) {}

	Settings::Types getType() const { return type; }
	static void setModel(Model *mod) { model = mod; };
	virtual bool process() = 0;

	virtual ~ICreature() = default;
};
