#pragma once

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <list>
#include <random>
#include <vector>

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

class ICreature;

namespace Settings {
// dimentions
const int N = 50;
const int M = 50;

// Sex[deprecated]
// enum Sex { Male = 0, Female };

// cellTypes
enum Types { Fox = 0, Bunny, Carrot, None }; // None is always in the end
// constexpr int PlantEdge = Carrot; // Everything above this constant is an
// animal[deprecated]

// LifeSpan
const int CarrotLifeSpan = 30;
const int FoxLifeSpan = 150;
const int BunnyLifeSpan = 100;

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
const double EmptyProb = 15.0;
const double CarrotProb = 20.0;
const double FoxProb = 30.0;
const double BunnyProb = 35.0;

const std::initializer_list<double> Probs = {EmptyProb, FoxProb, BunnyProb,
											 CarrotProb};

// Fabric
extern std::array<std::function<ICreature *(int, int)>, 3> Allocators;

extern std::array<sf::Color, 3> Colors;

} // namespace Settings