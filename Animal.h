#pragma once

#include "IAnimal.h"
#include "Model.h"
#include "Settings.h"

template <Settings::Types type, int LifeSpan, int NutVal, int Speed, int FOV,
		  Settings::Types Prey, Settings::Types Hunter>
class Animal : public IAnimal {
  protected:
	int satiety;
	int X;
	int Y;

	static int wave(int c, int i, int k) {
		int m = std::max(abs(i), abs(k));
		return std::max(c - m * m, 0);
	}

	double calcSat() const {
		if (satiety < 20)
			return 1.0;
		if (satiety > 80)
			return 0.0;
		return (satiety - 20.0) / 60.0;
	}

	double calcPart() const {
		if (age < 0.2 * LifeSpan || age > 0.8 * LifeSpan)
			return 0.0;
		double f = (age - 25.0) / 3.0;
		return 1.0 / (f * f + 1.0);
	}

	int findPath(int h, int p, int s) const {
		// Warning! This function uses only local information.

		int minX = std::max(X - FOV, 0);
		int maxX = std::min(X + FOV, Settings::N - 1);

		int minY = std::max(Y - FOV, 0);
		int maxY = std::min(Y + FOV, Settings::M - 1);

		int ways[9];

		std::array<int, Settings::None + 1> weights;
		weights[Hunter] = h;
		weights[Prey] = p;
		weights[type] = s;

		double threshold = calcPart();

		for (int x = minX; x <= maxX; ++x)
			for (int y = minY; y <= maxY; ++y)
				for (int r = 0; r < Settings::None; ++r)
					if (model->at(r, x, y) != nullptr) {
						int i = 0;
						for (int cx : {-1, 0, 1})
							for (int cy : {-1, 0, 1})
								if (X + cx > 0 && X + cx < Settings::N &&
									Y + cy > 0 && Y + cy < Settings::M &&
									(threshold != 0.0 ||
									 model->at(x - (X + cx), y - (Y + cy),
											   type) != nullptr)) {

									ways[i++] += wave(weights[r], x - (X + cx),
													  y - (Y + cy));

								} else
									ways[i++] = h * 30; //>5*5
					}

		return std::max_element(ways, ways + 9) - ways; // hack???
	};

  public:
	Animal(int i, int k) : satiety(100), X(i), Y(k){};

	virtual bool process() override {
		pass ^= true;

		if (++age > LifeSpan)
			return false; // too old

		if ((satiety -= 10) < 0)
			return false; // too hungry

		return true;
	}

	bool hasMoves() override {
		static int moves = 1;
		moves %= (Speed + 1);
		return moves++;
	}

	int move() {
		int w = findPath(-15, 13 * calcSat(), 11 * calcPart());

		int dx = w % 3 - 1;
		int dy = w / 3 - 1;

		if (model->at(type, X + dx, Y + dy) != nullptr) {
			if (calcPart() != 0.0)
				model->add(type, X, Y);
			return 4; // stay in current place
		}

		X += dx;
		Y += dy;

		if (calcSat() != 0.0 && model->at(Prey, X, Y) != nullptr) {
			model->kill(Prey, X, Y);
			satiety = std::min(satiety + NutVal, 100);
		}

		return w;
	}

	virtual ~Animal() = default;
};

using Bunny =
	Animal<Settings::Bunny, Settings::BunnyLifeSpan, Settings::CarrotNuVal,
		   Settings::BunnySpeed, Settings::BunnyFOV, Settings::Types::Carrot,
		   Settings::Types::Fox>;

using Fox = Animal<Settings::Fox, Settings::FoxLifeSpan, Settings::BunnyNuVal,
				   Settings::FoxSpeed, Settings::FoxFOV, Settings::Types::Bunny,
				   Settings::Types::None>;