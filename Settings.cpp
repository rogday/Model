#include "Settings.h"

#include "Bunny.h"
#include "Carrot.h"
#include "Fox.h"

Model *ICreature::model = nullptr;

// dimentions
int Settings::N = 0;
int Settings::M = 0;

// Fabric
std::array<std::function<ICreature *(int, int)>, 3> Settings::Allocators = {
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Carrot(); },
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Fox(i, k); },
	[](int i, int k) -> ICreature * { return (ICreature *)new ::Bunny(i, k); }};
