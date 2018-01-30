#include "Settings.h"

#include "Animal.h"
#include "Plant.h"

Model *ICreature::model = nullptr;

// Fabric
std::array<std::function<ICreature *(int, int, bool)>, 3> Settings::Allocators =
	{[](int i, int k, bool state) -> ICreature * {
		 return (ICreature *)new ::Fox(i, k, state);
	 },
	 [](int i, int k, bool state) -> ICreature * {
		 return (ICreature *)new ::Bunny(i, k, state);
	 },
	 [](int i, int k, bool state) -> ICreature * {
		 return (ICreature *)new ::Carrot(state);
	 }};

std::array<sf::Color, 3> Settings::Colors = {
	sf::Color(142, 61, 17), sf::Color(61, 17, 142), sf::Color(17, 142, 61)};

std::array<std::string, 3> Settings::Names = {"Fox", "Bunny", "Carrot"};

bool Settings::bounds(int x, int n) { return x >= 0 && x < n; }