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
	int X;
	int Y;

	static int wave(int c, int i, int k) {
		int m = std::max(abs(i), abs(k));
		return std::max(c - m * m, 0);
	}

	double calcSat() const {
		if (satiety < 20)
			return 0.0;
		if (satiety > 80)
			return 1.0;
		return (satiety - 20.0) / 60.0;
	}

	double calcPart() const {
		if (age < 0.2 * LifeSpan || age > 0.8 * LifeSpan)
			return 0.0;
		double f = abs(age - LifeSpan / 2);
		return 1.0 / (f + 1.0);
	}

	int findPath(int h, int p, int s) const {
		// Warning! This function uses only local information.

		int minX = std::max(X - FOV, 0);
		int maxX = std::min(X + FOV, Settings::N - 1);

		int minY = std::max(Y - FOV, 0);
		int maxY = std::min(Y + FOV, Settings::M - 1);

		int ways[9]{};

		std::array<int, Settings::None + 1> weights;
		weights[Hunter] = h;
		weights[Prey] = p;
		weights[type] = s;

		int i = 0;
		bool empty = true;
		for (int cy = -1; cy <= 1; ++cy)
			for (int cx = -1; cx <= 1; ++cx) {
				if (X + cx >= 0 && X + cx < Settings::N && Y + cy >= 0 &&
					Y + cy < Settings::M) {
					for (int x = minX; x <= maxX; ++x)
						for (int y = minY; y <= maxY; ++y)
							for (int r = 0; r < Settings::None; ++r)
								if (!model->empty(x, y, r)) {

									ways[i] += wave(weights[r], x - (X + cx),
													y - (Y + cy));
								}

				} else {
					ways[i] = INT32_MIN; //>5*5
					empty = false;
				}
				++i;
			}

		if (empty)
			return rand() % 9;

		return std::max_element(ways, ways + 9) - ways; // hack???
	};

  public:
	Animal(int i, int k)
		: satiety(80 + rand() % 21), sex(rand() % 2), delay(0), X(i), Y(k){};

	virtual bool process() override {
		pass ^= true;

		if (delay > 0)
			delay -= 1;

		if (++age > LifeSpan) {
#ifdef DEBUG
			std::cout << "too old" << std::endl;
#endif
			return false; // too old
		}

		if ((satiety -= 6) < 0) {
#ifdef DEBUG
			if (type == Settings::Bunny)
				std::cout << "I am fucking Bunny" << std::endl;
			std::cout << "too hungry" << std::endl;
#endif
			return false; // too hungry
		}

		return true;
	}

	bool hasMoves() override {
		static int moves = 1;
		moves %= (Speed + 1);
		return moves++;
	}

	int move() {
		int w = findPath(-35, 30 * calcSat(), 50 * calcPart());

		int dx = w % 3 - 1;
		int dy = w / 3 - 1;

		if (w != 4 && !model->empty(X + dx, Y + dy, type)) {
			if (calcPart() != 0.0 &&
				model->pinkTicket(X + dx, Y + dy, type, this)) {
#ifdef DEBUG
				std::cout << "population++" << std::endl;
#endif
				delay = Delay;
				model->add(X, Y, type);
			}
			return 4; // stay in current place
		}

		X += dx;
		Y += dy;

		if (calcSat() != 1.0 && !model->empty(X, Y, Prey)) {
#ifdef DEBUG
			std::cout << "Mnom-mnom" << std::endl;
#endif
			model->kill(X, Y, Prey);
			satiety = std::min(satiety + NutVal, 100);
		}

		return w;
	}

	bool getSex() override { return sex; };
	int getDelay() override { return delay; }

	virtual ~Animal() = default;
};

using Bunny =
	Animal<Settings::Bunny, Settings::BunnyLifeSpan, Settings::CarrotNuVal,
		   Settings::BunnySpeed, Settings::BunnyFOV, Settings::Types::Carrot,
		   Settings::Types::Fox, Settings::BunnyDelay>;

using Fox = Animal<Settings::Fox, Settings::FoxLifeSpan, Settings::BunnyNuVal,
				   Settings::FoxSpeed, Settings::FoxFOV, Settings::Types::Bunny,
				   Settings::Types::None, Settings::FoxDelay>;