#pragma once

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <array>
#include <cassert>
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

// Delay
const int FoxDelay = 6;
const int BunnyDelay = 2;

// Speed
// const int FoxSpeed = 5;
// const int BunnySpeed = 3;
const int FoxSpeed = 1;
const int BunnySpeed = 4;

// FOV
const int FoxFOV = 3;
const int BunnyFOV = 2;

// Nutritional Values
const int CarrotNuVal = 35;
const int BunnyNuVal = 80;

// Probablilities
const double EmptyProb = 10.0;
const double CarrotProb = 35.0;
const double FoxProb = 5.0;
const double BunnyProb = 50.0;

const std::initializer_list<double> Probs = {EmptyProb, FoxProb, BunnyProb,
											 CarrotProb};

// Fabric
extern std::array<std::function<ICreature *(int, int)>, 3> Allocators;

extern std::array<sf::Color, 3> Colors;

} // namespace Settings