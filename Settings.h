#pragma once

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <list>
#include <random>
#include <vector>

#include <iostream>

class ICreature;

namespace Settings {
// dimentions
const int N = 30;
const int M = 20;

// Sex[deprecated]
// enum Sex { Male = 0, Female };

// cellTypes
enum Types { Fox = 0, Bunny, Carrot, None }; // None is always in the end
// constexpr int PlantEdge = Carrot; // Everything above this constant is an
// animal[deprecated]

// LifeSpan
const int CarrotLifeSpan = 30;
const int FoxLifeSpan = 50;
const int BunnyLifeSpan = 40;

// Speed
const int FoxSpeed = 5;
const int BunnySpeed = 3;

// FOV
const int FoxFOV = 5;
const int BunnyFOV = 7;

// Nutritional Values
const int CarrotNuVal = 20;
const int BunnyNuVal = 40;

// Probablilities
const double EmptyProb = 35.0;
const double CarrotProb = 25.0;
const double FoxProb = 15.0;
const double BunnyProb = 25.0;

const std::initializer_list<double> Probs = {EmptyProb, CarrotProb, FoxProb,
											 BunnyProb};

// Fabric
extern std::array<std::function<ICreature *(int, int)>, 3> Allocators;

extern std::array<sf::Color, 3> Colors;

} // namespace Settings