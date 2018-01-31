#pragma once

#include "IAnimal.h"
#include "Model.h"
#include "Settings.h"

template <Settings::Types type, int LifeSpan, int NutVal, int Speed, int FOV,
		  Settings::Types Prey, Settings::Types Hunter, int Delay>
class Animal : public IAnimal {
  protected:
	int satiety;
	bool sex;
	int delay;
	const int X;
	const int Y;

	static int wave(int c, int i, int k) {
		int m = std::max(abs(i), abs(k));
		if (c > 0)
			return std::max(c - m * m, 0);
		else
			return -std::max(-c - m * m, 0);
	}

	double calcSat() const {
		if (satiety < 20)
			return 0.0;
		if (satiety > 80)
			return 1.0;
		return (satiety - 20.0) / 60.0;
	}

	double calcPart() const {
		double n = LifeSpan / 2;
		return std::sin((n + std::pow(age - n, 2)) / (M_PI * 2));

		double f = abs(age - LifeSpan / 5);
		return (LifeSpan / 10.0 / (f + 1.0)) - 0.6;
	}

	int findPath(int h, int p, int s) const {
		// Warning! This function uses only local information.

		/*
			FIX THAT.
		*/

		int minX = std::max(X - FOV, 0);
		int maxX = std::min(X + FOV, Settings::N - 1);

		int minY = std::max(Y - FOV, 0);
		int maxY = std::min(Y + FOV, Settings::M - 1);

		int ways[9]{};

		std::array<int, Settings::None + 1> weights{};
		weights[Hunter] = h;
		weights[Prey] = p;
		weights[type] = s;

		int i = 0;
		bool empty = true;
		for (int cy = -1; cy <= 1; ++cy)
			for (int cx = -1; cx <= 1; ++cx) {
				if (Settings::bounds(X + cx, Settings::N) &&
					Settings::bounds(Y + cy, Settings::M)) {
					for (int x = minX; x <= maxX; ++x)
						for (int y = minY; y <= maxY; ++y)
							for (int r = 0; r < Settings::None; ++r)
								if (!model->empty(x, y, r)) {
									empty = false;
									ways[i] += wave(weights[r], x - (X + cx),
													y - (Y + cy));
								}

				} else {
					ways[i] = INT32_MIN;
					empty = false;
				}
				++i;
			}

		if (empty)
			return rand() % 9;

		return std::max_element(ways, ways + 9) - ways; // hack???
	};

  public:
	Animal(int i, int k, bool state)
		: IAnimal(state), satiety(80 + rand() % 21), sex(rand() % 2), delay(0),
		  X(i), Y(k){};

	virtual void reset(bool pass) override {
		assert(!state);

#ifndef NDEBUG
		std::cout << "reset()" << std::endl;
#endif

		age = 0;
		state = true;
		satiety = 80 + rand() % 21;
		sex = rand() % 2;
		delay = 0;
		this->pass = pass;
	}

	virtual void swap(ICreature *another) override {
		Animal *ptr = dynamic_cast<Animal *>(another);

		std::swap(state, ptr->state);
		std::swap(satiety, ptr->satiety);
		std::swap(sex, ptr->sex);
		std::swap(delay, ptr->delay);
		std::swap(age, ptr->age);
		std::swap(pass, ptr->pass);
	}

	virtual bool process() override {
		assert(state);

		pass ^= true;

		if (delay > 0)
			delay -= 1;

		if (++age > LifeSpan) {
#ifndef NDEBUG
			std::cout << "too old" << std::endl;
#endif
			return false;
		}

		if ((satiety -= 4) < 0) {
#ifndef NDEBUG
			std::cout << "too hungry" << std::endl;
#endif
			return false;
		}

		return true;
	}

	bool hasMoves() override {
		static int moves = 1;
		moves %= (Speed + 1);
		return moves++;
	}

	int move() {
		int w = findPath(-FOV * FOV / 2, FOV * FOV * calcSat() / 3,
						 FOV * FOV * calcPart());

		int dx = w % 3 - 1;
		int dy = w / 3 - 1;

		if (w != 4 && !model->empty(X + dx, Y + dy, type)) {
			if (calcPart() > 0 &&
				model->pinkTicket(X + dx, Y + dy, type, this)) {
#ifndef NDEBUG
				std::cout << "population++" << std::endl;
#endif
				delay = Delay;
				model->add(X, Y, type);
			}
			return 4; // stay in current place
		}

		if (calcSat() != 1.0 && !model->empty(X, Y, Prey)) {
#ifndef NDEBUG
			std::cout << "Mnom-mnom" << std::endl;
#endif
			model->kill(X, Y, Prey);
			satiety = std::min(satiety + NutVal, 100);
		}

		return w;
	}

	bool getSex() const override { return sex; };
	int getDelay() const override { return delay; }
};

using Bunny =
	Animal<Settings::Bunny, Settings::BunnyLifeSpan, Settings::CarrotNuVal,
		   Settings::BunnySpeed, Settings::BunnyFOV, Settings::Types::Carrot,
		   Settings::Types::Fox, Settings::BunnyDelay>;

using Fox = Animal<Settings::Fox, Settings::FoxLifeSpan, Settings::BunnyNuVal,
				   Settings::FoxSpeed, Settings::FoxFOV, Settings::Types::Bunny,
				   Settings::Types::None, Settings::FoxDelay>;