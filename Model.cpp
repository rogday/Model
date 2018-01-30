#include "Model.h"

#include "IAnimal.h"
#include "IPlant.h"
#include <algorithm>

using namespace sf;

Model::Model()
	: field(Settings::N * Settings::M * Settings::None, nullptr), pass(false),
	  rects(Settings::N * Settings::M) {

	VideoMode vm = VideoMode::getDesktopMode();

	int size =
		std::min(vm.width, vm.height) / std::max(Settings::M, Settings::N) / 2;

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
		for (int k = 0; k < Settings::M; ++k) {
			rects[k * Settings::N + i].setSize(Vector2f(size, size));
			rects[k * Settings::N + i].setPosition(size * i, size * k);
			rects[k * Settings::N + i].setFillColor(Color::Black);
		}

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			if ((r = d(gen)) != 0)
				add(i + 1, k + 1, r - 1);
}

void Model::kill(int x, int y, int r) {
	Color clr = rects[y * Settings::N + x].getFillColor() - Settings::Colors[r];
	clr.a = 255;

	rects[y * Settings::N + x].setFillColor(clr);

	delete at(x, y, r);
	at(x, y, r) = nullptr;
}

bool bounds(int x, int n) { return x >= 0 && x < n; }

bool Model::pinkTicket(int x, int y, int r, IAnimal *two) {
	// return true;
	return dynamic_cast<IAnimal *>(at(x, y, r))->getSex() ^ two->getSex() &&
		   dynamic_cast<IAnimal *>(at(x, y, r))->getDelay() == 0 &&
		   two->getDelay() == 0;
}

void Model::add(int x, int y, int r) {
	// try to add child
	int nx, ny;
	for (int cy : {-1, 0, 1})
		for (int cx : {-1, 0, 1}) {
			nx = x + cx;
			ny = y + cy;
			if (bounds(nx, Settings::N) && bounds(ny, Settings::M) &&
				empty(nx, ny, r)) {
				at(nx, ny, r) = Settings::Allocators[r](nx, ny);
				Color clr = rects[ny * Settings::N + nx].getFillColor() +
							Settings::Colors[r];
				clr.a = 255;
				rects[ny * Settings::N + nx].setFillColor(clr);
				return;
			}
		}
}

ICreature *&Model::at(int x, int y, int r) {
	return field[Settings::N * Settings::M * r + y * Settings::N + x];
}

bool Model::empty(int x, int y, int r) { return at(x, y, r) == nullptr; }

void Model::move(int x, int y, int r, int way) {
	if (way == 4)
		return;

	int ni = x + way % 3 - 1;
	int nk = y + way / 3 - 1;

	assert(empty(ni, nk, r));

	at(ni, nk, r) = at(x, y, r);
	at(x, y, r) = nullptr;

	Color clr = rects[y * Settings::N + x].getFillColor() - Settings::Colors[r];
	clr.a = 255;

	rects[y * Settings::N + x].setFillColor(clr);

	clr = rects[nk * Settings::N + ni].getFillColor() + Settings::Colors[r];
	clr.a = 255;

	rects[nk * Settings::N + ni].setFillColor(clr);
}

void Model::processField() {
	IAnimal *ptr = nullptr;
	int coord = 0;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::discrete_distribution<> d({3, 97});

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			for (int r = 0; r < Settings::None; ++r)
				if (!empty(i, k, r)) {
					if (at(i, k, r)->ready(pass)) {
						if (!at(i, k, r)->process())
							kill(i, k, r);
						else if ((ptr = dynamic_cast<IAnimal *>(at(i, k, r))) !=
								 nullptr) {
							int dx = i;
							int dy = k;
							while (ptr->hasMoves()) {
								coord = ptr->move();
								move(dx, dy, r, coord);
								dx += coord % 3 - 1;
								dy += coord / 3 - 1;
							}
						}
					}
				} else {
					if (r == Settings::Carrot && d(gen) == 0)
						add(i + 1, k + 1, Settings::Carrot);
				}

	pass ^= true;
}

void Model::start() {
	Event event;

	while (window.isOpen()) {
		while (window.pollEvent(event))
			switch (event.type) {
			case Event::KeyPressed:
				if (event.key.code != Keyboard::Space)
					break;
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}

		window.clear(Color::White);

		for (auto &rect : rects) {
			Color clr = rect.getFillColor();

			if (rect.getFillColor() == Color::Black)
				rect.setFillColor(Color::White);

			window.draw(rect);

			rect.setFillColor(clr);
		}

		processField();

		window.display();
	}
}