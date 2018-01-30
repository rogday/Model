#include "Settings.h"

#include "Animal.h"
#include "Plant.h"

Model *ICreature::model = nullptr;

// Fabric
std::array<std::function<ICreature *(int, int)>, 3> Settings::Allocators = {
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Fox(i, k); },
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Bunny(i, k); },
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Carrot(); }};

std::array<sf::Color, 3> Settings::Colors = {
	sf::Color(142, 61, 17), sf::Color(61, 17, 142), sf::Color(17, 142, 61)};
