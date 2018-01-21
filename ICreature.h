#pragma once

class Model;

class ICreature {
  protected:
	static Model *model;
	int age;
	bool pass;

  public:
	ICreature() : age(0), pass(false) {}

	static void setModel(Model *mod) { model = mod; };
	virtual bool process() = 0;
	bool ready(bool Pass) { return Pass == pass; };

	virtual ~ICreature() = default;
};
