#include "Model.h"

#include "IAnimal.h"
#include "IPlant.h"

Model::Model()
	: field(Settings::N * Settings::M * Settings::None, nullptr), pass(false) {

	ICreature::setModel(this);

	std::random_device rd;
	std::mt19937 gen(rd());

	std::discrete_distribution<> d(Settings::Probs);

	int r = 0;

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			if ((r = d(gen)) != 0)
				at(i, k, r - 1) = Settings::Allocators[r - 1](i, k);
}

void Model::kill(Settings::Types type, int x, int y) {
	delete at(x, y, type);
	at(x, y, type) = nullptr;
}

void Model::add(Settings::Types type, int x, int y) {
	at(x, y, type) = Settings::Allocators[type](x, y);
}

ICreature *&Model::at(int x, int y, int r) {
	return field[Settings::N * Settings::M * r + y * Settings::N + x];
}

void Model::processField() {
	IAnimal *ptr = nullptr;
	int coord = 0;

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			for (int r = 0; r < Settings::None; ++r)
				if (at(i, k, r) && at(i, k, r)->ready(pass)) {
					if (!at(i, k, r)->process()) {
						delete at(i, k, r);
						at(i, k, r) = nullptr;
					} else if ((ptr = dynamic_cast<IAnimal *>(at(i, k, r))) !=
							   nullptr) {
						int dx = i, dy = k;
						while (ptr->hasMoves()) {
							coord = ptr->move();

							at(dx, dy, r) = nullptr;

							dx += coord % 3 - 1;
							dy += coord / 3 - 1;

							at(dx, dy, r) = ptr;
						}
					}
				}
}