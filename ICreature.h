#pragma once

class Model;

class ICreature {
  protected:
	static Model *model;
	int age;
	bool state;
	bool pass;

  public:
	ICreature(bool state) : age(0), state(state), pass(false) {}

	static void setModel(Model *mod) { model = mod; };
	virtual bool process() = 0;
	virtual void reset(bool) = 0;
	virtual void swap(ICreature *) = 0;
	void die() { state = false; };

	bool ready(bool Pass) const { return Pass == pass; };
	bool alive() const { return state; }

	virtual ~ICreature() = default;
};
