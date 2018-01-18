#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <list>
#include <random>
#include <vector>

class ICreature;

using Cell = std::list<ICreature *>; // Multiple creatures in one cell
using Map = std::vector<std::vector<Cell>>;

namespace Settings {
// dimentions
extern int N;
extern int M;

// Sex[deprecated]
// enum Sex { Male = 0, Female };

// cellTypes
enum Types { Fox = 0, Bunny, Carrot, None }; // None is always in the end
// constexpr int PlantEdge = Carrot; // Everything above this constant is an
// animal[deprecated]

// LifeSpan
constexpr int CarrotLifeSpan = 30;
constexpr int FoxLifeSpan = 50;
constexpr int BunnyLifeSpan = 40;

// Speed
constexpr int FoxSpeed = 5;
constexpr int BunnySpeed = 3;

// FOV
constexpr int FoxFOV = 5;
constexpr int BunnyFOV = 7;

// Nutritional Values
constexpr int CarrotNuVal = 20;
constexpr int BunnyNuVal = 40;

// Probablilities
constexpr double EmptyProb = 15.0;
constexpr double CarrotProb = 35.0;
constexpr double FoxProb = 20.0;
constexpr double BunnyProb = 30.0;

constexpr std::initializer_list<double> Probs = {EmptyProb, CarrotProb, FoxProb,
												 BunnyProb};

// Fabric
extern std::array<std::function<::ICreature *(int, int)>, 3> Allocators;

} // namespace Settings