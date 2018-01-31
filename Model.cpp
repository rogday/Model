#include "Model.h"

#include "IAnimal.h"
#include "IPlant.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace sf;

Model::Model()
	: field(Settings::N * Settings::M * Settings::None, nullptr), pass(false),
	  rects(Settings::N * Settings::M), born(Settings::None, 0),
	  died(Settings::None, 0) {

	VideoMode vm = VideoMode::getDesktopMode();

	int size = std::min(vm.width, vm.height) /
			   std::max(Settings::M, Settings::N) * Settings::SizeScale;

	window.create(sf::VideoMode(size * Settings::N, size * Settings::M),
				  "Life simulator", Style::Titlebar | Style::Close);
	window.setPosition(Vector2i(vm.width / 2 - size * Settings::N / 2,
								vm.height / 2 - size * Settings::M / 2));
	window.setFramerateLimit(Settings::FPS);

	ICreature::setModel(this);

	std::random_device rd;
	std::mt19937 gen(rd());

	std::discrete_distribution<> d(Settings::Probs);

	int r = 0;

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k) {
			rat(i, k).setSize(Vector2f(size, size));
			rat(i, k).setPosition(size * i, size * k);
			rat(i, k).setFillColor(Color::Black);
		}

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k) {
			for (r = 0; r < Settings::None; ++r)
				at(i, k, r) = Settings::Allocators[r](i, k, false);

			if ((r = d(gen)) != 0)
				add(i + 1, k + 1, r - 1);
		}
}

void Model::kill(int x, int y, int r) {
	Color clr = rat(x, y).getFillColor() - Settings::Colors[r];
	clr.a = 255;

	rat(x, y).setFillColor(clr);
	at(x, y, r)->die();
	++died[r];
}

bool Model::pinkTicket(int x, int y, int r, IAnimal *two) {
	return dynamic_cast<IAnimal *>(at(x, y, r))->getSex() ^ two->getSex() &&
		   dynamic_cast<IAnimal *>(at(x, y, r))->getDelay() == 0 &&
		   two->getDelay() == 0;
}

void Model::add(int x, int y, int r) {
	int nx, ny;
	for (int cy : {-1, 0, 1})
		for (int cx : {-1, 0, 1}) {
			nx = x + cx;
			ny = y + cy;
			if (Settings::bounds(nx, Settings::N) &&
				Settings::bounds(ny, Settings::M) && empty(nx, ny, r)) {
				at(nx, ny, r)->reset(pass);
				Color clr = rat(nx, ny).getFillColor() + Settings::Colors[r];
				clr.a = 255;
				rat(nx, ny).setFillColor(clr);
				++born[r];
				return;
			}
		}
}

void Model::move(int x, int y, int r, int way) {
	int ni = x + way % 3 - 1;
	int nk = y + way / 3 - 1;

	assert(Settings::bounds(ni, Settings::N) &&
		   Settings::bounds(nk, Settings::M) && empty(ni, nk, r));

	at(x, y, r)->swap(at(ni, nk, r));

	Color clr = rat(x, y).getFillColor() - Settings::Colors[r];
	clr.a = 255;

	rat(x, y).setFillColor(clr);

	clr = rat(ni, nk).getFillColor() + Settings::Colors[r];
	clr.a = 255;

	rat(ni, nk).setFillColor(clr);
}

void Model::processField() {
	IAnimal *ptr = nullptr;
	int coord = 0;

	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			for (int r = 0; r < Settings::None; ++r)
				if (!empty(i, k, r)) {
					if (at(i, k, r)->ready(pass)) {
						int dx = i;
						int dy = k;

						if (at(dx, dy, r)->process())
							while ((ptr = dynamic_cast<IAnimal *>(
										at(dx, dy, r))) != nullptr &&
								   ptr->hasMoves()) {
								coord = ptr->move();
								if (coord == 4)
									break;
								move(dx, dy, r, coord);
								dx += coord % 3 - 1;
								dy += coord / 3 - 1;
							}
						else
							kill(dx, dy, r);
					}
				} else {
					if (r == Settings::Carrot && Settings::xorshf96() % 101 < 6)
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

			if (clr == Color::Black)
				rect.setFillColor(Color::White);
			else if (clr == Color(220, 220, 220))
				rect.setFillColor(Color::Black);

			window.draw(rect);

			rect.setFillColor(clr);
		}

		processField();

		window.display();
	}
}

Model::~Model() {
	for (int i = 0; i < Settings::N; ++i)
		for (int k = 0; k < Settings::M; ++k)
			for (int r = 0; r < Settings::None; ++r)
				delete at(i, k, r);
}

void Model::prettyPrint() {
	std::cout << "Born / Died / Alive:" << std::endl << std::endl;

	int max = std::max_element(Settings::Names.begin(), Settings::Names.end(),
							   [](std::string &a, std::string &b) {
								   return a.size() < b.size();
							   })
				  ->size();

	for (int i = 0; i < Settings::None; ++i) {
		std::string &str = Settings::Names[i];
		std::cout << str << std::setw(max - str.size() + 14) << born[i] << " / "
				  << died[i] << " / " << born[i] - died[i] << std::endl;
	}
}