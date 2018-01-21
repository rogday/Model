#include "Model.h"

#include "IAnimal.h"
#include "IPlant.h"
#include <algorithm>

using namespace sf;

Model::Model()
	: field(Settings::N * Settings::M * Settings::None, nullptr), pass(false) {

	VideoMode vm = VideoMode::getDesktopMode();

	int size =
		std::min(vm.width, vm.height) / std::max(Settings::M, Settings::N);

	window.create(sf::VideoMode(size * Settings::N, size * Settings::M),
				  "Life simulator", Style::Titlebar | Style::Close);
	window.setPosition(Vector2i(vm.width / 8, vm.height / 8));
	window.setFramerateLimit(75);

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
	// try to add child
	for (int cy = -1; cy <= 1; ++cy)
		for (int cx = -1; cx <= 1; ++cx) {
			if (x + cx >= 0 && x + cx < Settings::N && y + cy >= 0 &&
				y + cy < Settings::M && at(x + cx, y + cy, type) == nullptr)
				at(x + cx, y + cy, type) =
					Settings::Allocators[type](x + cx, y + cy);
		}
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
				if (at(i, k, r) != nullptr && at(i, k, r)->ready(pass)) {
					if (!at(i, k, r)->process())
						kill((Settings::Types)r, i, k);
					else if ((ptr = dynamic_cast<IAnimal *>(at(i, k, r))) !=
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

	pass ^= true;
}

void Model::start() {
	Event event;

	std::vector<RectangleShape> rects(Settings::N * Settings::M);
	int size = window.getSize().x / Settings::N;

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k) {
			rects[k * Settings::N + i] = RectangleShape(Vector2f(size, size));
			rects[k * Settings::N + i].setPosition(size * i, size * k);
		}

	while (window.isOpen()) {
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		window.clear(Color::White);

		for (int i = 0; i < Settings::N; ++i)
			for (int k = 0; k < Settings::M; ++k) {
				Color clr;
				for (int r = 0; r < Settings::None; ++r)
					if (at(i, k, r))
						clr += Settings::Colors[r];

				if (clr == Color::Black)
					clr = Color::White;
				rects[k * Settings::N + i].setFillColor(clr);
			}

		for (auto &rect : rects)
			window.draw(rect);

		processField();

		window.display();
	}
}