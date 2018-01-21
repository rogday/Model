#include "Settings.h"

#include "Animal.h"
#include "Plant.h"

Model *ICreature::model = nullptr;

// Fabric
std::array<std::function<ICreature *(int, int)>, 3> Settings::Allocators = {
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Carrot(); },
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Fox(i, k); },
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Bunny(i, k); }};
