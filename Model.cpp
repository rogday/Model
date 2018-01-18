#include "Model.h"

#include "Bunny.h"
#include "Carrot.h"
#include "Fox.h"

Model::Model(int n, int m) : field(n, std::vector<Cell>(m)) {
	srand(time(nullptr)); // for animals etc

	Settings::N = n;
	Settings::M = m;

	ICreature::setModel(this);

	std::random_device rd;
	std::mt19937 gen(rd());

	std::discrete_distribution<> d(Settings::Probs);

	int r = 0;

	for (int i = 0; i < n; ++i)
		for (int k = 0; k < m; ++k)
			if ((r = d(gen)) != 0)
				field[i][k].push_back(Settings::Allocators[r - 1](i, k));
}

void Model::kill(Settings::Types type, int x, int y) {
	auto it = get(type, x, y);
	if (it != (Cell::iterator) nullptr) {
		delete (*it);
		field[x][y].erase(it);
	}
}

void Model::add(Settings::Types type, int x, int y) {
	field[x][y].push_back(Settings::Allocators[type](x, y));
}

Cell::iterator Model::get(Settings::Types type, int x, int y) {
	auto it =
		std::find_if(field[x][y].begin(), field[x][y].end(),
					 [type](ICreature *cr) { return type == cr->getType(); });

	return (it == field[x][y].end()) ? ((Cell::iterator) nullptr) : (it);
}

void Model::processField() {
	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			for (auto it = field[i][k].begin(); it != field[i][k].end(); ++it)
				if (!(*it)->process()) {
					delete *it;
					it = field[i][k].erase(it);
				} else if ((*it)->getType() < Settings::PlantEdge) {
					while (((Animal *)(*it))->hasMoves()) {
						coord = ((Animal *)(*it))->move();
						// moving stuff
					}
				}
}